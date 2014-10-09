/*-
 * Copyright (c) 2006 Elad Efrat <elad@NetBSD.org>
 * Copyright (c) 2013-2014, by Oliver Pinter <oliver.pntr at gmail.com>
 * Copyright (c) 2014, by Shawn Webb <lattera at gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include "opt_compat.h"
#include "opt_pax.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/ktr.h>
#include <sys/imgact.h>
#include <sys/imgact_elf.h>
#include <sys/sysent.h>
#include <sys/stat.h>
#include <sys/proc.h>
#include <sys/elf_common.h>
#include <sys/mount.h>
#include <sys/pax.h>
#include <sys/sysctl.h>
#include <sys/vnode.h>
#include <sys/queue.h>
#include <sys/libkern.h>
#include <sys/jail.h>

#include <sys/mman.h>
#include <sys/libkern.h>
#include <sys/exec.h>
#include <sys/kthread.h>

#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_extern.h>

#include <machine/elf.h>

#include <security/mac_bsdextended/mac_bsdextended.h>

FEATURE(aslr, "Address Space Layout Randomization.");

int pax_aslr_status = PAX_FEATURE_OPTOUT;
int pax_aslr_debug = PAX_FEATURE_SIMPLE_DISABLED;

#ifdef PAX_ASLR_MAX_SEC
int pax_aslr_mmap_len = PAX_ASLR_DELTA_MMAP_MAX_LEN;
int pax_aslr_stack_len = PAX_ASLR_DELTA_STACK_MAX_LEN;
int pax_aslr_exec_len = PAX_ASLR_DELTA_EXEC_MAX_LEN;
#else
int pax_aslr_mmap_len = PAX_ASLR_DELTA_MMAP_DEF_LEN;
int pax_aslr_stack_len = PAX_ASLR_DELTA_STACK_DEF_LEN;
int pax_aslr_exec_len = PAX_ASLR_DELTA_EXEC_DEF_LEN;
#endif /* PAX_ASLR_MAX_SEC */

#ifdef COMPAT_FREEBSD32
int pax_aslr_compat_status = PAX_FEATURE_OPTOUT;
#ifdef PAX_ASLR_MAX_SEC
int pax_aslr_compat_mmap_len = PAX_ASLR_COMPAT_DELTA_MMAP_MAX_LEN;
int pax_aslr_compat_stack_len = PAX_ASLR_COMPAT_DELTA_STACK_MAX_LEN;
int pax_aslr_compat_exec_len = PAX_ASLR_COMPAT_DELTA_EXEC_MAX_LEN;
#else
int pax_aslr_compat_mmap_len = PAX_ASLR_COMPAT_DELTA_MMAP_MIN_LEN;
int pax_aslr_compat_stack_len = PAX_ASLR_COMPAT_DELTA_STACK_MIN_LEN;
int pax_aslr_compat_exec_len = PAX_ASLR_COMPAT_DELTA_EXEC_MIN_LEN;
#endif /* PAX_ASLR_MAX_SEC */
#endif /* COMPAT_FREEBSD32 */

TUNABLE_INT("hardening.pax.aslr.status", &pax_aslr_status);
TUNABLE_INT("hardening.pax.aslr.mmap_len", &pax_aslr_mmap_len);
TUNABLE_INT("hardening.pax.aslr.debug", &pax_aslr_debug);
TUNABLE_INT("hardening.pax.aslr.stack_len", &pax_aslr_stack_len);
TUNABLE_INT("hardening.pax.aslr.exec_len", &pax_aslr_exec_len);
#ifdef COMPAT_FREEBSD32
TUNABLE_INT("hardening.pax.aslr.compat.status", &pax_aslr_compat_status);
TUNABLE_INT("hardening.pax.aslr.compat.mmap", &pax_aslr_compat_mmap_len);
TUNABLE_INT("hardening.pax.aslr.compat.stack", &pax_aslr_compat_stack_len);
TUNABLE_INT("hardening.pax.aslr.compat.exec", &pax_aslr_compat_exec_len);
#endif

#ifdef PAX_SYSCTLS

SYSCTL_DECL(_hardening_pax);

/*
 * sysctls and tunables
 */
static int sysctl_pax_aslr_debug(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_aslr_status(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_aslr_mmap(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_aslr_stack(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_aslr_exec(SYSCTL_HANDLER_ARGS);


SYSCTL_NODE(_hardening_pax, OID_AUTO, aslr, CTLFLAG_RD, 0,
    "Address Space Layout Randomization.");

SYSCTL_PROC(_hardening_pax_aslr, OID_AUTO, status,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_aslr_status, "I",
    "Restrictions status. "
    "0 - disabled, "
    "1 - opt-in,  "
    "2 - opt-out, "
    "3 - force enabled");

SYSCTL_PROC(_hardening_pax_aslr, OID_AUTO, debug,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_aslr_debug, "I",
    "ASLR debug mode");

SYSCTL_PROC(_hardening_pax_aslr, OID_AUTO, mmap_len,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_aslr_mmap, "I",
    "Number of bits randomized for mmap(2) calls. "
    "32 bit: [8,20] 64 bit: [16,32]");

SYSCTL_PROC(_hardening_pax_aslr, OID_AUTO, stack_len,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_aslr_stack, "I",
    "Number of bits randomized for the stack. "
    "32 bit: [6,12] 64 bit: [12,21]");

SYSCTL_PROC(_hardening_pax_aslr, OID_AUTO, exec_len,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON|CTLFLAG_SECURE,
    NULL, 0, sysctl_pax_aslr_exec, "I",
    "Number of bits randomized for the PIE exec base. "
    "32 bit: [6,20] 64 bit: [12,21]");

static int
sysctl_pax_aslr_status(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ? pr->pr_pax_aslr_status : pax_aslr_status;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	switch (val) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		if ((pr == NULL) || (pr == &prison0))
			pax_aslr_status = val;

		if (pr != NULL) {
			prison_lock(pr);
			pr->pr_pax_aslr_status = val;
			prison_unlock(pr);
		}
		break;
	default:
		return (EINVAL);
	}

	return (0);
}

static int
sysctl_pax_aslr_debug(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr=NULL;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ? pr->pr_pax_aslr_debug : pax_aslr_debug;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	switch (val) {
	case 0:
	case 1:
		break;
	default:
		return (EINVAL);

	}

	if ((pr == NULL) || (pr == &prison0))
		pax_aslr_debug = val;

	if (pr != NULL) {
		prison_lock(pr);
		pr->pr_pax_aslr_debug = val;
		prison_unlock(pr);
	}

	return (0);
}

static int
sysctl_pax_aslr_mmap(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr=NULL;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ? pr->pr_pax_aslr_mmap_len : pax_aslr_mmap_len;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	if (val < PAX_ASLR_DELTA_MMAP_MIN_LEN ||
	    val > PAX_ASLR_DELTA_MMAP_MAX_LEN)
		return (EINVAL);

	if ((pr == NULL) || (pr == &prison0))
		pax_aslr_mmap_len = val;

	if (pr != NULL) {
		prison_lock(pr);
		pr->pr_pax_aslr_mmap_len = val;
		prison_unlock(pr);
	}

	return (0);
}

static int
sysctl_pax_aslr_stack(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr=NULL;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ? pr->pr_pax_aslr_stack_len : pax_aslr_stack_len;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	if (val < PAX_ASLR_DELTA_STACK_MIN_LEN ||
	    val > PAX_ASLR_DELTA_STACK_MAX_LEN)
		return (EINVAL);

	if ((pr == NULL) || (pr == &prison0))
		pax_aslr_stack_len = val;

	if (pr != NULL) {
		prison_lock(pr);
		pr->pr_pax_aslr_stack_len = val;
		prison_unlock(pr);
	}

	return (0);
}

static int
sysctl_pax_aslr_exec(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr=NULL;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ? pr->pr_pax_aslr_exec_len : pax_aslr_exec_len;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	if (val < PAX_ASLR_DELTA_EXEC_MIN_LEN ||
	    val > PAX_ASLR_DELTA_EXEC_MAX_LEN)
		return (EINVAL);

	if ((pr == NULL) || (pr == &prison0))
		pax_aslr_exec_len = val;

	if (pr != NULL) {
		prison_lock(pr);
		pr->pr_pax_aslr_exec_len = val;
		prison_unlock(pr);
	}

	return (0);
}

/* COMPAT_FREEBSD32 and linuxulator. */
#ifdef COMPAT_FREEBSD32
static int sysctl_pax_aslr_compat_status(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_aslr_compat_mmap(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_aslr_compat_stack(SYSCTL_HANDLER_ARGS);
static int sysctl_pax_aslr_compat_exec(SYSCTL_HANDLER_ARGS);

SYSCTL_NODE(_hardening_pax_aslr, OID_AUTO, compat, CTLFLAG_RD, 0,
    "Setting for COMPAT_FREEBSD32 and linuxulator.");

SYSCTL_PROC(_hardening_pax_aslr_compat, OID_AUTO, status,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON,
    NULL, 0, sysctl_pax_aslr_compat_status, "I",
    "Restrictions status. "
    "0 - disabled, "
    "1 - enabled,  "
    "2 - global enabled, "
    "3 - force global enabled");

SYSCTL_PROC(_hardening_pax_aslr_compat, OID_AUTO, mmap_len,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON,
    NULL, 0, sysctl_pax_aslr_compat_mmap, "I",
    "Number of bits randomized for mmap(2) calls. "
    "32 bit: [8,16]");

SYSCTL_PROC(_hardening_pax_aslr_compat, OID_AUTO, stack_len,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON,
    NULL, 0, sysctl_pax_aslr_compat_stack, "I",
    "Number of bits randomized for the stack. "
    "32 bit: [6,12]");

SYSCTL_PROC(_hardening_pax_aslr_compat, OID_AUTO, exec_len,
    CTLTYPE_INT|CTLFLAG_RWTUN|CTLFLAG_PRISON,
    NULL, 0, sysctl_pax_aslr_compat_exec, "I",
    "Number of bits randomized for the PIE exec base. "
    "32 bit: [6,12]");

static int
sysctl_pax_aslr_compat_status(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ?pr->pr_pax_aslr_compat_status : pax_aslr_compat_status;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || (req->newptr == NULL))
		return (err);

	switch (val) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		if ((pr == NULL) || (pr == &prison0))
			pax_aslr_compat_status = val;

		if (pr != NULL) {
			prison_lock(pr);
			pr->pr_pax_aslr_compat_status = val;
			prison_unlock(pr);
		}
		break;
	default:
		return (EINVAL);
	}

	return (0);
}

static int
sysctl_pax_aslr_compat_mmap(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ? pr->pr_pax_aslr_compat_mmap_len : pax_aslr_compat_mmap_len;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	if (val < PAX_ASLR_COMPAT_DELTA_MMAP_MIN_LEN ||
	    val > PAX_ASLR_COMPAT_DELTA_MMAP_MAX_LEN)
		return (EINVAL);

	if ((pr == NULL) || (pr == &prison0))
		pax_aslr_compat_mmap_len = val;

	if (pr != NULL) {
		prison_lock(pr);
		pr->pr_pax_aslr_compat_mmap_len = val;
		prison_unlock(pr);
	}

	return (0);
}

static int
sysctl_pax_aslr_compat_stack(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	val = (pr != NULL) ? pr->pr_pax_aslr_compat_stack_len : pax_aslr_compat_stack_len;
	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	if (val < PAX_ASLR_COMPAT_DELTA_STACK_MIN_LEN ||
	    val > PAX_ASLR_COMPAT_DELTA_STACK_MAX_LEN)
		return (EINVAL);

	if ((pr == NULL) || (pr == &prison0))
		pax_aslr_compat_stack_len = val;

	if (pr != NULL) {
		prison_lock(pr);
		pr->pr_pax_aslr_compat_stack_len = val;
		prison_unlock(pr);
	}

	return (0);
}

static int
sysctl_pax_aslr_compat_exec(SYSCTL_HANDLER_ARGS)
{
	struct prison *pr;
	int err, val;

	pr = pax_get_prison(req->td->td_proc);

	if (pr != NULL)
		val = pr->pr_pax_aslr_compat_exec_len;
	else
		val = pax_aslr_compat_exec_len;

	err = sysctl_handle_int(oidp, &val, sizeof(int), req);
	if (err || !req->newptr)
		return (err);

	if (val < PAX_ASLR_COMPAT_DELTA_EXEC_MIN_LEN ||
	    val > PAX_ASLR_COMPAT_DELTA_EXEC_MAX_LEN)
		return (EINVAL);

	if ((pr == NULL) || (pr == &prison0))
		pax_aslr_compat_exec_len = val;

	if (pr != NULL) {
		prison_lock(pr);
		pr->pr_pax_aslr_compat_exec_len = val;
		prison_unlock(pr);
	}

	return (0);
}

#endif /* COMPAT_FREEBSD32 */
#endif /* PAX_SYSCTLS */


/*
 * ASLR functions
 */

static void
pax_aslr_sysinit(void)
{
	switch (pax_aslr_status) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		break;
	default:
		printf("[PAX ASLR] WARNING, invalid PAX settings in loader.conf!"
		    " (pax_aslr_status = %d)\n", pax_aslr_status);
		pax_aslr_status = PAX_FEATURE_FORCE_ENABLED;
		break;
	}
	printf("[PAX ASLR] status: %s\n", pax_status_str[pax_aslr_status]);
	printf("[PAX ASLR] mmap: %d bit\n", pax_aslr_mmap_len);
	printf("[PAX ASLR] exec base: %d bit\n", pax_aslr_exec_len);
	printf("[PAX ASLR] stack: %d bit\n", pax_aslr_stack_len);
}
SYSINIT(pax_aslr, SI_SUB_PAX, SI_ORDER_SECOND, pax_aslr_sysinit, NULL);

bool
pax_aslr_active(struct proc *p)
{
	u_int flags;
	bool ret;

	ret = pax_get_flags(p, &flags);
	if (ret != 0)
		/*
		 * invalid flags, we should force ASLR
		 */
		return (true);

	CTR3(KTR_PAX, "%s: pid = %d p_pax = %x",
	    __func__, p->p_pid, flags);

	if ((flags & PAX_NOTE_ASLR) == PAX_NOTE_ASLR)
		return (true);

	if ((flags & PAX_NOTE_NOASLR) == PAX_NOTE_NOASLR)
		return (false);

	return (true);
}

void
_pax_aslr_init(struct vmspace *vm, struct proc *p)
{
	struct prison *pr;

	KASSERT(vm != NULL, ("%s: vm is null", __func__));

	pr = pax_get_prison(p);
	if (pr != NULL) {
		vm->vm_aslr_delta_mmap = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_DELTA_MMAP_LSB,
		    pr->pr_pax_aslr_mmap_len);

		vm->vm_aslr_delta_stack = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_DELTA_STACK_LSB,
		    pr->pr_pax_aslr_stack_len);
		vm->vm_aslr_delta_stack = ALIGN(vm->vm_aslr_delta_stack);

		vm->vm_aslr_delta_exec = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_DELTA_EXEC_LSB,
		    pr->pr_pax_aslr_exec_len);
	} else {
		vm->vm_aslr_delta_mmap = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_DELTA_MMAP_LSB,
		    pax_aslr_mmap_len);

		vm->vm_aslr_delta_stack = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_DELTA_STACK_LSB,
		    pax_aslr_stack_len);
		vm->vm_aslr_delta_stack = ALIGN(vm->vm_aslr_delta_stack);

		vm->vm_aslr_delta_exec = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_DELTA_EXEC_LSB,
		    pax_aslr_exec_len);
	}

	CTR2(KTR_PAX, "%s: vm_aslr_delta_mmap=%p\n",
	    __func__, (void *)vm->vm_aslr_delta_mmap);
	CTR2(KTR_PAX, "%s: vm_aslr_delta_stack=%p\n",
	    __func__, (void *)vm->vm_aslr_delta_stack);
	CTR2(KTR_PAX, "%s: vm_aslr_delta_exec=%p\n",
	    __func__, (void *)vm->vm_aslr_delta_exec);
}

#ifdef COMPAT_FREEBSD32
static void
pax_compat_aslr_sysinit(void)
{
	switch (pax_aslr_compat_status) {
	case PAX_FEATURE_DISABLED:
	case PAX_FEATURE_OPTIN:
	case PAX_FEATURE_OPTOUT:
	case PAX_FEATURE_FORCE_ENABLED:
		break;
	default:
		printf("[PAX ASLR (compat)] WARNING, invalid PAX settings in loader.conf! "
		    "(pax_aslr_compat_status = %d)\n", pax_aslr_compat_status);
		pax_aslr_compat_status = PAX_FEATURE_FORCE_ENABLED;
		break;
	}
	printf("[PAX ASLR (compat)] status: %s\n", pax_status_str[pax_aslr_compat_status]);
	printf("[PAX ASLR (compat)] mmap: %d bit\n", pax_aslr_compat_mmap_len);
	printf("[PAX ASLR (compat)] exec base: %d bit\n", pax_aslr_compat_exec_len);
	printf("[PAX ASLR (compat)] stack: %d bit\n", pax_aslr_compat_stack_len);
}
SYSINIT(pax_compat_aslr, SI_SUB_PAX, SI_ORDER_SECOND, pax_compat_aslr_sysinit, NULL);

void
_pax_aslr_init32(struct vmspace *vm, struct proc *p)
{
	struct prison *pr;

	KASSERT(vm != NULL, ("%s: vm is null", __func__));

	pr = pax_get_prison(p);
	if (pr != NULL) {
		vm->vm_aslr_delta_mmap = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_COMPAT_DELTA_MMAP_LSB,
		    pr->pr_pax_aslr_compat_mmap_len);

		vm->vm_aslr_delta_stack = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_COMPAT_DELTA_STACK_LSB,
		    pr->pr_pax_aslr_compat_stack_len);
		vm->vm_aslr_delta_stack = ALIGN(vm->vm_aslr_delta_stack);

		vm->vm_aslr_delta_exec = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_COMPAT_DELTA_EXEC_LSB,
		    pr->pr_pax_aslr_compat_exec_len);
	} else {
		vm->vm_aslr_delta_mmap = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_COMPAT_DELTA_MMAP_LSB,
		    pax_aslr_compat_mmap_len);

		vm->vm_aslr_delta_stack = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_COMPAT_DELTA_STACK_LSB,
		    pax_aslr_compat_stack_len);
		vm->vm_aslr_delta_stack = ALIGN(vm->vm_aslr_delta_stack);

		vm->vm_aslr_delta_exec = PAX_ASLR_DELTA(arc4random(),
		    PAX_ASLR_COMPAT_DELTA_EXEC_LSB,
		    pax_aslr_compat_exec_len);
	}

	CTR2(KTR_PAX, "%s: vm_aslr_delta_mmap=%p\n",
	    __func__, (void *)vm->vm_aslr_delta_mmap);
	CTR2(KTR_PAX, "%s: vm_aslr_delta_stack=%p\n",
	    __func__, (void *)vm->vm_aslr_delta_stack);
	CTR2(KTR_PAX, "%s: vm_aslr_delta_exec=%p\n",
	    __func__, (void *)vm->vm_aslr_delta_exec);
}
#endif

void
pax_aslr_init(struct image_params *imgp)
{
	struct vmspace *vm;
	struct proc *p;

	KASSERT(imgp != NULL, ("%s: imgp is null", __func__));
	p = imgp->proc;

	if (!pax_aslr_active(p))
		return;

	vm = p->p_vmspace;

	if (imgp->sysent->sv_pax_aslr_init != NULL)
		imgp->sysent->sv_pax_aslr_init(vm, p);
}

void
pax_aslr_mmap(struct proc *p, vm_offset_t *addr, vm_offset_t orig_addr, int flags)
{
	int len_32bit;

	if (!pax_aslr_active(p))
		return;

#ifdef COMPAT_FREEBSD32
	len_32bit = pax_aslr_compat_mmap_len;
#else
	len_32bit = PAX_ASLR_COMPAT_DELTA_MMAP_MIN_LEN;
#endif

	if (!(flags & MAP_FIXED) && ((orig_addr == 0) || !(flags & MAP_ANON))) {
		CTR4(KTR_PAX, "%s: applying to %p orig_addr=%p flags=%x\n",
		    __func__, (void *)*addr, (void *)orig_addr, flags);

#ifdef MAP_32BIT
		if (flags & MAP_32BIT)
			*addr += PAX_ASLR_DELTA(arc4random(),
			    PAX_ASLR_COMPAT_DELTA_MMAP_LSB,
			    len_32bit);
		else
#endif /* MAP_32BIT */
			*addr += p->p_vmspace->vm_aslr_delta_mmap;
		CTR2(KTR_PAX, "%s: result %p\n", __func__, (void *)*addr);
	} else
		CTR4(KTR_PAX, "%s: not applying to %p orig_addr=%p flags=%x\n",
		    __func__, (void *)*addr, (void *)orig_addr, flags);
}

void
pax_aslr_stack(struct proc *p, uintptr_t *addr)
{
	uintptr_t orig_addr;

	if (!pax_aslr_active(td->td_proc))
		return;

	orig_addr = *addr;
	*addr -= p->p_vmspace->vm_aslr_delta_stack;
	CTR3(KTR_PAX, "%s: orig_addr=%p, new_addr=%p\n",
	    __func__, (void *)orig_addr, (void *)*addr);
}

u_int
pax_aslr_setup_flags(struct image_params *imgp, u_int mode)
{
	struct prison *pr;
	u_int flags, status;

	flags = 0;
	status = 0;

	pr = pax_get_prison(imgp->proc);
	if (pr != NULL)
		status = pr->pr_pax_aslr_status;
	else
		status = pax_aslr_status;

	if (status == PAX_FEATURE_DISABLED) {
		flags &= ~PAX_NOTE_ASLR;
		flags |= PAX_NOTE_NOASLR;

		return (flags);
	}

	if (status == PAX_FEATURE_FORCE_ENABLED) {
		flags |= PAX_NOTE_ASLR;
		flags &= ~PAX_NOTE_NOASLR;

		return (flags);
	}

	if (status == PAX_FEATURE_OPTIN) {
		if (mode & MBI_ASLR_ENABLED) {
			flags |= PAX_NOTE_ASLR;
			flags &= ~PAX_NOTE_NOASLR;
		} else {
			flags &= ~PAX_NOTE_ASLR;
			flags |= PAX_NOTE_NOASLR;
			pax_log_aslr(imgp->proc, __func__,
	"ASLR is opt-in, and executable don't have enabled ASLR!\n");
			pax_ulog_aslr(NULL,
	"ASLR is opt-in, and executable don't have enabled ASLR!\n");
		}

		return (flags);
	}

	if (status == PAX_FEATURE_OPTOUT) {
		if (mode & MBI_ASLR_DISABLED) {
			flags &= ~PAX_NOTE_ASLR;
			flags |= PAX_NOTE_NOASLR;
			pax_log_aslr(imgp->proc, __func__,
	 "ASLR is opt-out, and executable explicitly disabled ASLR!\n");
			pax_ulog_aslr(NULL,
	 "ASLR is opt-out, and executable explicitly disabled ASLR!\n");
		} else {
			flags |= PAX_NOTE_ASLR;
			flags &= ~PAX_NOTE_NOASLR;
		}

		return (flags);
	}

	/*
	 * unknown status, force ASLR
	 */
	flags |= PAX_NOTE_ASLR;
	flags &= ~PAX_NOTE_NOASLR;

	return (flags);
}
