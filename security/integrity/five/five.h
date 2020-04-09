/*
 * This code is based on IMA's code
 *
 * Copyright (C) 2016 Samsung Electronics, Inc.
 *
 * Egor Uleyskiy, <e.uleyskiy@samsung.com>
 * Viacheslav Vovchenko <v.vovchenko@samsung.com>
 * Yevgen Kopylov <y.kopylov@samsung.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __LINUX_FIVE_H
#define __LINUX_FIVE_H

#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/security.h>
#include <linux/hash.h>
#include <linux/audit.h>
#include <linux/workqueue.h>

#include "../integrity.h"
#include "five_cert.h"
#include "five_crypto.h"

/* set during initialization */
extern int five_hash_algo;
struct worker_context {
	struct work_struct data_work;
	struct task_integrity *tint;
};

struct f_signature_task {
	struct task_integrity *tint;
	struct file *file;
};

struct f_signature_context {
	struct work_struct data_work;
	struct f_signature_task payload;
};

/* Internal FIVE function definitions */
int five_init(void);

/* FIVE policy related functions */
enum five_hooks {
	FILE_CHECK = 1,
	MMAP_CHECK,
	BPRM_CHECK,
	POST_SETATTR
};

int five_appraise_measurement(struct task_struct *task, int func,
			      struct integrity_iint_cache *iint,
			      struct file *file,
			      struct five_cert *cert);
enum five_file_integrity five_get_cache_status(
					struct integrity_iint_cache *iint);

int five_read_xattr(struct dentry *dentry, char **xattr_value);
int five_check_params(struct task_struct *task, struct file *file);
const char *five_d_path(struct path *path, char **pathbuf);

int five_digsig_verify(struct five_cert *cert,
			    const char *digest, int digestlen);
void __init five_load_built_x509(void);
int __init five_keyring_init(void);
#endif
