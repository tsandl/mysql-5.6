/***********************************************************************

Copyright (c) 2010, Oracle and/or its affiliates. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

***********************************************************************/
/**************************************************//**
@file innodb_api.h

Created 03/15/2011      Jimmy Yang
*******************************************************/

#ifndef innodb_api_h
#define innodb_api_h

#include <memcached/types.h>
#include "api0api.h"
#include "innodb_engine.h"
#include "assert.h"

typedef struct mci_column {
	char*		m_str;
	int		m_len;
	uint64_t	m_digit;
	bool		m_is_str;
	bool		m_enabled;
} mci_column_t;

/** We would need to fetch 5 values from each key value rows if they
are available. They are the "key", "value", "cas", "exp" and "flag" */
#define	MCI_ITEM_TO_GET		5

enum mci_item_idx {
	MCI_COL_KEY,
	MCI_COL_VALUE,
	MCI_COL_CAS,
	MCI_COL_EXP,
	MCI_COL_FLAG
};

typedef struct mci_items {
	mci_column_t	mci_item[MCI_ITEM_TO_GET];
	mci_column_t*	mci_add_value;
	int		mci_add_num;
} mci_item_t;

/** Following are callback function defines for InnoDB APIs */
typedef
ib_err_t
(*CB_OPEN_TABLE)(
/*=============*/
	const char*	name,
	ib_trx_t	ib_trx,
	ib_crsr_t*	ib_crsr);

typedef 
ib_err_t
(*CB_READ_ROW)(
/*===========*/
        ib_crsr_t	ib_crsr,
        ib_tpl_t	ib_tpl);

typedef
ib_err_t
(*CB_INSERT_ROW)(
/*=============*/
	ib_crsr_t	ib_crsr,
	const ib_tpl_t	ib_tpl);

typedef
ib_err_t
(*CB_CURSOR_DELETE_ROW)(
/*=================*/
	ib_crsr_t	ib_crsr);

typedef
ib_err_t
(*CB_CURSOR_UPDATE_ROW)(
/*====================*/
	ib_crsr_t	ib_crsr,
	const ib_tpl_t	ib_old_tpl,
	const ib_tpl_t	ib_new_tpl);

typedef
ib_err_t
(*CB_CURSOR_MOVETO)(
/*================*/
	ib_crsr_t	ib_crsr,
	ib_tpl_t	ib_tpl,
	ib_srch_mode_t	ib_srch_mode);

typedef
ib_tpl_t
(*CB_SEC_SEARCH_TUPLE_CREATE)(
/*==========================*/
	ib_crsr_t	ib_crsr) ;

typedef
ib_tpl_t
(*CB_SEC_READ_TUPLE_CREATE)(
/*========================*/
	ib_crsr_t	ib_crsr);

typedef
void
(*CB_TUPLE_DELETE)(
/*===============*/
	ib_tpl_t	ib_tpl);

typedef
ib_err_t
(*CB_TUPLE_COPY)(
/*=============*/
	ib_tpl_t	ib_dst_tpl,
	const ib_tpl_t	ib_src_tpl);

typedef
ib_err_t
(*CB_TUPLE_READ_U32)(
/*=================*/
	ib_tpl_t	ib_tpl,
	ib_ulint_t	i,
	ib_u32_t*	ival) ;

typedef
ib_err_t
(*CB_TUPLE_WRITE_U32)(
/*==================*/
	ib_tpl_t	ib_tpl,
	int		col_no,
	ib_u32_t	val) ;

typedef
ib_err_t
(*CB_TUPLE_READ_U64)(
/*=================*/
	ib_tpl_t	ib_tpl,
	ib_ulint_t	i,
	ib_u64_t*	ival) ;

typedef
ib_err_t
(*CB_TUPLE_WRITE_U64)(
/*==================*/
	ib_tpl_t	ib_tpl,
	int		col_no,
	ib_u64_t	val);

typedef
ib_err_t
(*CB_TUPLE_READ_I32)(
/*=================*/
	ib_tpl_t	ib_tpl,
	ib_ulint_t	i,
	ib_i32_t*	ival);

typedef
ib_err_t
(*CB_TUPLE_WRITE_I32)(
/*==================*/
	ib_tpl_t	ib_tpl,
	int		col_no,
	ib_i32_t	val);

typedef
ib_err_t
(*CB_TUPLE_READ_I64)(
/*=================*/
	ib_tpl_t	ib_tpl,
	ib_ulint_t	i,
	ib_i64_t*	ival);

typedef
ib_err_t
(*CB_TUPLE_WRITE_I64)(
/*==================*/
	ib_tpl_t	ib_tpl,
	int		col_no,
	ib_i64_t	val);

typedef
ib_err_t
(*CB_COL_SET_VALUE)(
/*================*/
	ib_tpl_t	ib_tpl,
	ib_ulint_t	col_no,
	const void*	src,
	ib_ulint_t	len) ;

typedef
const void*
(*CB_COL_GET_VALUE)(
/*================*/
	ib_tpl_t	ib_tpl,
	ib_ulint_t	i);

typedef
ib_ulint_t
(*CB_COL_GET_META)(
/*===============*/
	ib_tpl_t	ib_tpl,
	ib_ulint_t	i,
	ib_col_meta_t*	ib_col_meta);

typedef
ib_trx_t
(*CB_TRX_BEGIN)(
/*============*/
	ib_trx_level_t	ib_trx_level);

typedef
ib_err_t
(*CB_TRX_COMMIT)(
/*=============*/
	ib_trx_t	ib_trx);

typedef
ib_err_t
(*CB_TRX_ROLLBACK)(
/*===============*/
	ib_trx_t	ib_trx);

typedef
ib_err_t
(*CB_TRX_START)(
/*===============*/
	ib_trx_t	ib_trx,
	ib_trx_level_t	ib_trx_level);

typedef
ib_trx_state_t
(*CB_TRX_STATE)(
/*===============*/
	ib_trx_t	ib_trx);

typedef
ib_trx_state_t
(*CB_TRX_RELEASE)(
/*===============*/
	ib_trx_t	ib_trx);

typedef
ib_ulint_t
(*CB_TUPLE_GET_N_COLS)(
/*===================*/
	const ib_tpl_t	ib_tpl);

typedef
void
(*CB_CURSOR_SET_MATCH_MODE)(
/*========================*/
	ib_crsr_t	ib_crsr,
	ib_match_mode_t	match_mode);

typedef
ib_err_t
(*CB_CURSOR_LOCK)(
/*==============*/
	ib_crsr_t	ib_crsr,
	ib_lck_mode_t	ib_lck_mode);

typedef
ib_err_t
(*CB_CURSOR_CLOSE)(
/*===============*/
	ib_crsr_t	ib_crsr);

typedef
void*
(*CB_OPEN_TABLE_BY_NAME)(
/*=====================*/
	const char*	name);

typedef
ib_err_t
(*CB_CURSOR_NEW_TRX)(
/*=================*/
	ib_crsr_t	ib_crsr,
	ib_trx_t	ib_trx);

typedef
ib_err_t
(*CB_CURSOR_RESET)(
/*===============*/
	ib_crsr_t	ib_crsr);

typedef
char*
(*CB_COL_GET_NAME)(
/*===============*/
	ib_crsr_t	ib_crsr,
	ib_ulint_t	i);

typedef
ib_err_t
(*CB_TABLE_TRUNCATE)(
/*=================*/
	const char*	table_name,
	ib_id_t*	table_id);

typedef
ib_err_t
(*CB_CURSOR_FIRST)(
/*===============*/
        ib_crsr_t       ib_crsr);

typedef
ib_err_t
(*CB_CURSOR_LAST)(
/*==============*/
        ib_crsr_t       ib_crsr);

typedef
ib_err_t
(*CB_CURSOR_OPEN_INDEX_USING_NAME)(
/*===============================*/
	ib_crsr_t	ib_open_crsr,
	const char*	index_name,
	ib_crsr_t*	ib_crsr,
	int*		idx_type,
        ib_id_t*	idx_id);

CB_OPEN_TABLE			ib_cb_open_table;
CB_READ_ROW			ib_cb_read_row;
CB_INSERT_ROW			ib_cb_insert_row;
CB_CURSOR_DELETE_ROW		ib_cb_delete_row;
CB_CURSOR_UPDATE_ROW		ib_cb_update_row;
CB_CURSOR_MOVETO		ib_cb_moveto;
CB_SEC_SEARCH_TUPLE_CREATE	ib_cb_search_tuple_create;
CB_SEC_READ_TUPLE_CREATE	ib_cb_read_tuple_create;
CB_TUPLE_DELETE			ib_cb_tuple_delete;
CB_TUPLE_COPY			ib_cb_tuple_copy;
CB_TUPLE_READ_U32		ib_cb_tuple_read_u32;
CB_TUPLE_WRITE_U32		ib_cb_tuple_write_u32;
CB_TUPLE_WRITE_U64		ib_cb_tuple_write_u64;
CB_TUPLE_READ_I32		ib_cb_tuple_read_i32;
CB_TUPLE_WRITE_I32		ib_cb_tuple_write_i32;
CB_TUPLE_WRITE_I64		ib_cb_tuple_write_i64;
CB_TUPLE_READ_I64		ib_cb_tuple_read_i64;
CB_TUPLE_READ_U64		ib_cb_tuple_read_u64;
CB_COL_SET_VALUE		ib_cb_col_set_value;
CB_COL_GET_VALUE		ib_cb_col_get_value;
CB_COL_GET_META			ib_cb_col_get_meta;
CB_TRX_BEGIN			ib_cb_trx_begin;
CB_TRX_COMMIT			ib_cb_trx_commit;
CB_TRX_ROLLBACK			ib_cb_trx_rollback;
CB_TRX_START			ib_cb_trx_start;
CB_TRX_STATE			ib_cb_trx_state;
CB_TRX_RELEASE			ib_cb_trx_release;
CB_TUPLE_GET_N_COLS		ib_cb_tuple_get_n_cols;
CB_CURSOR_SET_MATCH_MODE	ib_cb_cursor_set_match_mode;
CB_CURSOR_LOCK			ib_cb_cursor_lock;
CB_CURSOR_CLOSE			ib_cb_cursor_close;
CB_CURSOR_NEW_TRX		ib_cb_cursor_new_trx;
CB_OPEN_TABLE_BY_NAME		ib_cb_open_table_by_name;
CB_CURSOR_RESET			ib_cb_cursor_reset;
CB_COL_GET_NAME			ib_cb_col_get_name;
CB_TABLE_TRUNCATE		ib_cb_table_truncate;
CB_CURSOR_FIRST			ib_cb_cursor_first;
CB_CURSOR_LAST			ib_cb_cursor_last;
CB_CURSOR_OPEN_INDEX_USING_NAME	ib_cb_cursor_open_index_using_name;

/*********************************************************************
Open a table and return a cursor for the table. */
ib_err_t
innodb_api_begin(
/*=============*/
	innodb_engine_t*engine,		/*!< in: InnoDB Memcached engine */
	const char*	dbname,		/*!< in: database name */
	const char*	name,		/*!< in: table name */
	ib_trx_t	ib_trx,		/*!< in: transaction */
	ib_crsr_t*	crsr,		/*!< out: innodb cursor */
	ib_crsr_t*	idx_crsr,	/*!< out: innodb index cursor */
	ib_lck_mode_t	lock_mode);	/*!< in:  lock mode */


/*************************************************************//**
Position a row accord to key, and fetch value if needed
@return DB_SUCCESS if successful otherwise, error code */
ib_err_t
innodb_api_search(
/*==============*/
	innodb_engine_t*	engine,	/*!< in: InnoDB Memcached engine */
	innodb_conn_data_t*	cursor_data,/*!< in: cursor info */
	ib_crsr_t*		crsr,	/*!< out: cursor used to seacrh */
	const char*		key,	/*!< in: key to search */
	int			len,	/*!< in: key length */
	mci_item_t*		item,	/*!< in: result */
	ib_tpl_t*		r_tpl,	/*!< in: tpl for other DML operations */
	bool			sel_only); /*!< in: for select only */

/*************************************************************//**
Insert a row
@return DB_SUCCESS if successful otherwise, error code */
ib_err_t
innodb_api_insert(
/*==============*/
	innodb_engine_t*	engine,	/*!< in: InnoDB Memcached engine */
	innodb_conn_data_t*	cursor_data,/*!< in: cursor info */
	const char*		key,	/*!< in: value to insert */
	int			len,	/*!< in: value length */
	uint32_t		val_len,/*!< in: value length */
	uint64_t		exp,	/*!< in: expire time */
	uint64_t*		cas,	/*!< in/out: cas value */
        uint64_t		flags);	/*!< in: flags */


/*************************************************************//**
Delete a row, implements the "remove" command
@return DB_SUCCESS if successful otherwise, error code */
ENGINE_ERROR_CODE
innodb_api_delete(
/*==============*/
	innodb_engine_t*	engine,	/*!< in: InnoDB Memcached engine */
	innodb_conn_data_t*	cursor_data,/*!< in: cursor info */
	const char*		key,	/*!< in: value to insert */
	int			len);	/*!< in: value length */

/*************************************************************//**
Update a row with arithmetic operation
@return DB_SUCCESS if successful otherwise ENGINE_NOT_STORED*/
ENGINE_ERROR_CODE
innodb_api_arithmetic(
/*==================*/
	innodb_engine_t*	engine,	/*!< in: InnoDB Memcached engine */
	innodb_conn_data_t*	cursor_data,/*!< in: cursor info */
	const char*		key,	/*!< in: key values */
	int			len,	/*!< in: key length */
	int			delta,	/*!< in: value to add or subtract */
	bool			increment,/*!< in: increment or decrement */
	uint64_t*		cas,	/*!< out: cas */
	rel_time_t		exp_time, /*!< in: expire time */
	bool			create,	/*!< in: whether to create new entry
					if not found */
	uint64_t		initial,/*!< in: initialize value */
	uint64_t*		result);/*!< out: result value */

/*************************************************************//**
This is the interface to following commands:
	1) add
	2) replace
	3) append
	4) prepend
	5) set
	6) cas
@return ENGINE_SUCCESS if successful otherwise, error code */
ENGINE_ERROR_CODE
innodb_api_store(
/*=============*/
	innodb_engine_t*	engine,	/*!< in: InnoDB Memcached engine */
	innodb_conn_data_t*	cursor_data,/*!< in: cursor info */
	const char*		key,	/*!< in: key value */
	int			len,	/*!< in: key length */
	uint32_t		val_len,/*!< in: value length */
	uint64_t		exp,	/*!< in: expire time */
	uint64_t*		cas,	/*!< out: cas value */
	uint64_t		input_cas,/*!< in: cas value supplied by user */
	uint64_t		flags,	/*!< in: flags */
	ENGINE_STORE_OPERATION	op);	/*!< in: Operations */

/*********************************************************************
Implement the flush_all command */
ENGINE_ERROR_CODE
innodb_api_flush(
/*=============*/
	const char*	dbname,	/*!< in: database name */
	const char*	name);	/*!< in: table name */

/*************************************************************//**
Get current time */
uint64_t
mci_get_time(void);
/*==============*/

typedef enum conn_op_type {
        CONN_OP_READ,
        CONN_OP_WRITE,
        CONN_OP_DELETE
} op_type_t;

/*************************************************************//**
reset the cursor */
void
innodb_api_cursor_reset(
/*====================*/
	innodb_engine_t*	engine,		 /*!< in: InnoDB Memcached
						engine */
	innodb_conn_data_t*	conn_data,	/*!< in/out: cursor affiliated
						with a connection */
	op_type_t               op_type);	/*!< in: type of DML performed */
#endif
