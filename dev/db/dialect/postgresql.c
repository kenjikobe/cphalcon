
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2012 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_phalcon.h"
#include "phalcon.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/operators.h"

/**
 * Phalcon\Db\Dialect\Postgresql
 *
 * Generates database specific SQL for the PostgreSQL RBDM
 */

/**
 * Generates the SQL for a PostgreSQL LIMIT clause
 *
 * @param string $sqlQuery
 * @param int $number
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, limit){

	zval *sql_query = NULL, *number = NULL, *limit = NULL;
	zval *r0 = NULL, *r1 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &sql_query, &number) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_FUNC_PARAMS_1(r0, "is_numeric", number);
	if (zend_is_true(r0)) {
		PHALCON_INIT_VAR(limit);
		PHALCON_CALL_FUNC_PARAMS_1(limit, "intval", number);
		
		PHALCON_ALLOC_ZVAL_MM(r1);
		PHALCON_CONCAT_VSV(r1, sql_query, " LIMIT ", limit);
		
		RETURN_CTOR(r1);
	} else {
		
		RETURN_CCTOR(sql_query);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Returns a SQL modified with a FOR UPDATE clause
 *
 * @param string $sqlQuery
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, forUpdate){

	zval *sql_query = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &sql_query) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CONCAT_VS(r0, sql_query, " FOR UPDATE");
	
	RETURN_CTOR(r0);
}

/**
 * Returns a SQL modified with a LOCK IN SHARE MODE clause
 *
 * @param string $sqlQuery
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, sharedLock){

	zval *sql_query = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &sql_query) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CONCAT_VS(r0, sql_query, " LOCK IN SHARE MODE");
	
	RETURN_CTOR(r0);
}

/**
 * Gets a list of columns
 *
 * @param array $columnList
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, getColumnList){

	zval *column_list = NULL, *str_list = NULL, *column = NULL;
	zval *r0 = NULL, *r1 = NULL;
	zval *c0 = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &column_list) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_INIT_VAR(str_list);
	array_init(str_list);
	if (!phalcon_valid_foreach(column_list TSRMLS_CC)) {
		return;
	}
	
	ah0 = Z_ARRVAL_P(column_list);
	zend_hash_internal_pointer_reset_ex(ah0, &hp0);
	fes_46b6_0:
		if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
			goto fee_46b6_0;
		}
		
		PHALCON_INIT_VAR(column);
		ZVAL_ZVAL(column, *hd, 1, 0);
		PHALCON_INIT_VAR(r0);
		PHALCON_CONCAT_SVS(r0, "\"", column, "\"");
		phalcon_array_append(&str_list, r0, PH_SEPARATE TSRMLS_CC);
		zend_hash_move_forward_ex(ah0, &hp0);
		goto fes_46b6_0;
	fee_46b6_0:
	if(0){}
	
	PHALCON_INIT_VAR(c0);
	ZVAL_STRING(c0, ", ", 1);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	phalcon_fast_join(r1, c0, column_list TSRMLS_CC);
	RETURN_CTOR(r1);
}

/**
 * Gets the column name in PostgreSQL
 *
 * @param Phalcon\Db\Column $column
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, getColumnDefinition){

	zval *column = NULL, *size = NULL, *column_sql = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL;
	zval *r7 = NULL, *r8 = NULL, *r9 = NULL;
	zval *t0 = NULL, *t1 = NULL, *t2 = NULL, *t3 = NULL, *t4 = NULL, *t5 = NULL, *t6 = NULL;
	zval *t7 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &column) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(column) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Column definition must be an instance of Phalcon_Db_Column");
		return;
	}
	
	PHALCON_INIT_VAR(size);
	PHALCON_CALL_METHOD(size, column, "getsize", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD(r0, column, "gettype", PH_NO_CHECK);
	
	PHALCON_INIT_VAR(t0);
	ZVAL_LONG(t0, 0);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	is_equal_function(r1, r0, t0 TSRMLS_CC);
	if (zend_is_true(r1)) {
		PHALCON_INIT_VAR(column_sql);
		ZVAL_STRING(column_sql, "INT", 1);
		goto se_46b6_1;
	}
	
	PHALCON_INIT_VAR(t1);
	ZVAL_LONG(t1, 1);
	
	PHALCON_ALLOC_ZVAL_MM(r2);
	is_equal_function(r2, r0, t1 TSRMLS_CC);
	if (zend_is_true(r2)) {
		PHALCON_INIT_VAR(column_sql);
		ZVAL_STRING(column_sql, "DATE", 1);
		goto se_46b6_1;
	}
	
	PHALCON_INIT_VAR(t2);
	ZVAL_LONG(t2, 2);
	
	PHALCON_ALLOC_ZVAL_MM(r3);
	is_equal_function(r3, r0, t2 TSRMLS_CC);
	if (zend_is_true(r3)) {
		PHALCON_INIT_VAR(column_sql);
		PHALCON_CONCAT_SVS(column_sql, "CHARACTER VARYING(", size, ")");
		goto se_46b6_1;
	}
	
	PHALCON_INIT_VAR(t3);
	ZVAL_LONG(t3, 3);
	
	PHALCON_ALLOC_ZVAL_MM(r4);
	is_equal_function(r4, r0, t3 TSRMLS_CC);
	if (zend_is_true(r4)) {
		PHALCON_ALLOC_ZVAL_MM(r5);
		PHALCON_CALL_METHOD(r5, column, "getscale", PH_NO_CHECK);
		PHALCON_INIT_VAR(column_sql);
		PHALCON_CONCAT_SVSVS(column_sql, "NUMERIC(", size, ",", r5, ")");
		goto se_46b6_1;
	}
	
	PHALCON_INIT_VAR(t4);
	ZVAL_LONG(t4, 4);
	
	PHALCON_ALLOC_ZVAL_MM(r6);
	is_equal_function(r6, r0, t4 TSRMLS_CC);
	if (zend_is_true(r6)) {
		PHALCON_INIT_VAR(column_sql);
		ZVAL_STRING(column_sql, "TIMESTAMP", 1);
		goto se_46b6_1;
	}
	
	PHALCON_INIT_VAR(t5);
	ZVAL_LONG(t5, 5);
	
	PHALCON_ALLOC_ZVAL_MM(r7);
	is_equal_function(r7, r0, t5 TSRMLS_CC);
	if (zend_is_true(r7)) {
		PHALCON_INIT_VAR(column_sql);
		PHALCON_CONCAT_SVS(column_sql, "CHARACTER(", size, ")");
		goto se_46b6_1;
	}
	
	PHALCON_INIT_VAR(t6);
	ZVAL_LONG(t6, 6);
	
	PHALCON_ALLOC_ZVAL_MM(r8);
	is_equal_function(r8, r0, t6 TSRMLS_CC);
	if (zend_is_true(r8)) {
		PHALCON_INIT_VAR(column_sql);
		ZVAL_STRING(column_sql, "TEXT", 1);
		goto se_46b6_1;
	}
	
	PHALCON_INIT_VAR(t7);
	ZVAL_LONG(t7, 7);
	
	PHALCON_ALLOC_ZVAL_MM(r9);
	is_equal_function(r9, r0, t7 TSRMLS_CC);
	if (zend_is_true(r9)) {
		PHALCON_INIT_VAR(column_sql);
		ZVAL_STRING(column_sql, "FLOAT", 1);
		goto se_46b6_1;
	}
	PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Unrecognized PostgreSQL data type");
	return;
	se_46b6_1:
	
	RETURN_CTOR(column_sql);
}

/**
 * Generates SQL to add a column to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\Column $column
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, addColumn){

	zval *table_name = NULL, *schema_name = NULL, *column = NULL, *sql = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL;
	zval *t0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &column) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(column) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Column parameter must be an instance of Phalcon_Db_Column");
		return;
	}
	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` ADD ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE `", table_name, "` ADD ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD(r0, column, "getname", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	PHALCON_CALL_SELF_PARAMS_1(r1, this_ptr, "getcolumndefinition", column);
	
	PHALCON_ALLOC_ZVAL_MM(r2);
	PHALCON_CONCAT_SVSV(r2, "`", r0, "` ", r1);
	phalcon_concat_self(&sql, r2 TSRMLS_CC);
	
	PHALCON_ALLOC_ZVAL_MM(r3);
	PHALCON_CALL_METHOD(r3, column, "isnotnull", PH_NO_CHECK);
	if (zend_is_true(r3)) {
		PHALCON_INIT_VAR(t0);
		ZVAL_STRING(t0, " NOT NULL", 1);
		phalcon_concat_self(&sql, t0 TSRMLS_CC);
	}
	
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to modify a column in a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\Column $column
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, modifyColumn){

	zval *table_name = NULL, *schema_name = NULL, *column = NULL, *sql = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL;
	zval *t0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &column) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(column) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Column parameter must be an instance of Phalcon_Db_Column");
		return;
	}
	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` MODIFY ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE `", table_name, "` MODIFY ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD(r0, column, "getname", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	PHALCON_CALL_SELF_PARAMS_1(r1, this_ptr, "getcolumndefinition", column);
	
	PHALCON_ALLOC_ZVAL_MM(r2);
	PHALCON_CONCAT_SVSV(r2, "`", r0, "` ", r1);
	phalcon_concat_self(&sql, r2 TSRMLS_CC);
	
	PHALCON_ALLOC_ZVAL_MM(r3);
	PHALCON_CALL_METHOD(r3, column, "isnotnull", PH_NO_CHECK);
	if (zend_is_true(r3)) {
		PHALCON_INIT_VAR(t0);
		ZVAL_STRING(t0, " NOT NULL", 1);
		phalcon_concat_self(&sql, t0 TSRMLS_CC);
	}
	
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to delete a column from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $columnName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, dropColumn){

	zval *table_name = NULL, *schema_name = NULL, *column_name = NULL;
	zval *sql = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &column_name) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE `", schema_name, "`.`", table_name, "` DROP COLUMN ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE `", table_name, "` DROP COLUMN ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CONCAT_SVS(r0, "`", column_name, "`");
	phalcon_concat_self(&sql, r0 TSRMLS_CC);
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\Index $index
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, addIndex){

	zval *table_name = NULL, *schema_name = NULL, *index = NULL, *sql = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &index) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(index) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Index parameter must be an instance of Phalcon_Db_Index");
		return;
	}
	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE ", schema_name, ".", table_name, " ADD INDEX ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE ", table_name, " ADD INDEX ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD(r0, index, "getname", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	PHALCON_CALL_METHOD(r1, index, "getcolumns", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r2);
	PHALCON_CALL_SELF_PARAMS_1(r2, this_ptr, "getcolumnlist", r1);
	
	PHALCON_ALLOC_ZVAL_MM(r3);
	PHALCON_CONCAT_SVSVS(r3, "\"", r0, "\" (", r2, ")");
	phalcon_concat_self(&sql, r3 TSRMLS_CC);
	
	RETURN_CTOR(sql);
}

/**
  * Generates SQL to delete an index from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $indexName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, dropIndex){

	zval *table_name = NULL, *schema_name = NULL, *index_name = NULL;
	zval *sql = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &index_name) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE ", schema_name, ".", table_name, " DROP INDEX ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE ", table_name, " DROP INDEX ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CONCAT_SVS(r0, "\"", index_name, "\"");
	phalcon_concat_self(&sql, r0 TSRMLS_CC);
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to add the primary key to a table
 *
 * @param string\$tableName
 * @param string\$schemaName
 * @param Phalcon\Db\Index $index
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, addPrimaryKey){

	zval *table_name = NULL, *schema_name = NULL, *index = NULL, *sql = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &index) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(index) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Index parameter must be an instance of Phalcon_Db_Index");
		return;
	}
	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE ", schema_name, ".", table_name, " ADD PRIMARY KEY ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE ", table_name, " ADD PRIMARY KEY ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD(r0, index, "getcolumns", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	PHALCON_CALL_SELF_PARAMS_1(r1, this_ptr, "getcolumnlist", r0);
	
	PHALCON_ALLOC_ZVAL_MM(r2);
	PHALCON_CONCAT_SVS(r2, "(", r1, ")");
	phalcon_concat_self(&sql, r2 TSRMLS_CC);
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to delete primary key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, dropPrimaryKey){

	zval *table_name = NULL, *schema_name = NULL, *sql = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &table_name, &schema_name) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE ", schema_name, ".", table_name, " DROP PRIMARY KEY");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE ", table_name, " DROP PRIMARY KEY");
	}
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to add an index to a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param Phalcon\Db\Reference $reference
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, addForeignKey){

	zval *table_name = NULL, *schema_name = NULL, *reference = NULL, *sql = NULL;
	zval *referenced_schema = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL;
	zval *r7 = NULL, *r8 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &reference) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (Z_TYPE_P(reference) != IS_OBJECT) {
		PHALCON_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Reference parameter must be an instance of Phalcon\\Db\\Reference");
		return;
	}
	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE ", schema_name, ".", table_name, " ADD FOREIGN KEY ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE ", table_name, " ADD FOREIGN KEY ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CALL_METHOD(r0, reference, "getname", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r1);
	PHALCON_CALL_METHOD(r1, reference, "getcolumns", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r2);
	PHALCON_CALL_SELF_PARAMS_1(r2, this_ptr, "getcolumnlist", r1);
	
	PHALCON_ALLOC_ZVAL_MM(r3);
	PHALCON_CONCAT_SVSVS(r3, "\"", r0, "\"(", r2, ") REFERENCES ");
	phalcon_concat_self(&sql, r3 TSRMLS_CC);
	
	PHALCON_INIT_VAR(referenced_schema);
	PHALCON_CALL_METHOD(referenced_schema, reference, "getreferencedschema", PH_NO_CHECK);
	if (zend_is_true(referenced_schema)) {
		PHALCON_ALLOC_ZVAL_MM(r4);
		PHALCON_CONCAT_SVS(r4, "", referenced_schema, ".");
		phalcon_concat_self(&sql, r4 TSRMLS_CC);
	}
	
	PHALCON_ALLOC_ZVAL_MM(r5);
	PHALCON_CALL_METHOD(r5, reference, "getreferencedtable", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r6);
	PHALCON_CALL_METHOD(r6, reference, "getreferencedcolumns", PH_NO_CHECK);
	
	PHALCON_ALLOC_ZVAL_MM(r7);
	PHALCON_CALL_SELF_PARAMS_1(r7, this_ptr, "getcolumnlist", r6);
	
	PHALCON_ALLOC_ZVAL_MM(r8);
	PHALCON_CONCAT_SVSVS(r8, "", r5, "(", r7, ")");
	phalcon_concat_self(&sql, r8 TSRMLS_CC);
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to delete a foreign key from a table
 *
 * @param string $tableName
 * @param string $schemaName
 * @param string $referenceName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, dropForeignKey){

	zval *table_name = NULL, *schema_name = NULL, *reference_name = NULL;
	zval *sql = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &reference_name) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVSVS(sql, "ALTER TABLE ", schema_name, ".", table_name, " DROP FOREIGN KEY ");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "ALTER TABLE ", table_name, " DROP FOREIGN KEY ");
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CONCAT_SVS(r0, "\"", reference_name, "\"");
	phalcon_concat_self(&sql, r0 TSRMLS_CC);
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to add the table creation options
 *
 * @param array $definition
 * @return array
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, _getTableOptions){

	zval *definition = NULL;
	zval *a0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &definition) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	PHALCON_ALLOC_ZVAL_MM(a0);
	array_init(a0);
	
	RETURN_CTOR(a0);
}

/**
 * Generates SQL to create a table in PostgreSQL
 *
 * @param string $tableName
 * @param string $schemaName
 * @param array $definition
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, createTable){

	zval *table_name = NULL, *schema_name = NULL, *definition = NULL;
	zval *table = NULL, *temporary = NULL, *sql = NULL, *create_lines = NULL, *column = NULL;
	zval *column_line = NULL, *index = NULL, *index_name = NULL, *column_list = NULL;
	zval *reference = NULL, *reference_sql = NULL;
	zval *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL;
	zval *r7 = NULL, *r8 = NULL, *r9 = NULL, *r10 = NULL, *r11 = NULL, *r12 = NULL, *r13 = NULL;
	zval *r14 = NULL, *r15 = NULL, *r16 = NULL, *r17 = NULL, *r18 = NULL, *r19 = NULL, *r20 = NULL;
	zval *r21 = NULL, *r22 = NULL, *r23 = NULL;
	zval *t0 = NULL, *t1 = NULL;
	zval *c0 = NULL;
	HashTable *ah0, *ah1, *ah2;
	HashPosition hp0, hp1, hp2;
	zval **hd;
	int eval_int;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table_name, &schema_name, &definition) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(table);
		PHALCON_CONCAT_VSV(table, schema_name, ".", table_name);
	} else {
		PHALCON_CPY_WRT(table, table_name);
	}
	
	PHALCON_INIT_VAR(temporary);
	ZVAL_BOOL(temporary, 0);
	eval_int = phalcon_array_isset_string(definition, SL("options")+1);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(r0);
		phalcon_array_fetch_string(&r0, definition, SL("options"), PH_NOISY_CC);
		eval_int = phalcon_array_isset_string(r0, SL("temporary")+1);
		if (eval_int) {
			PHALCON_ALLOC_ZVAL_MM(r1);
			phalcon_array_fetch_string(&r1, definition, SL("options"), PH_NOISY_CC);
			PHALCON_ALLOC_ZVAL_MM(r2);
			phalcon_array_fetch_string(&r2, r1, SL("temporary"), PH_NOISY_CC);
			if (zend_is_true(r2)) {
				PHALCON_INIT_VAR(temporary);
				ZVAL_BOOL(temporary, 1);
			}
		}
	}
	
	if (zend_is_true(temporary)) {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "CREATE TEMPORARY TABLE ", table, " (\n\t");
	} else {
		PHALCON_INIT_VAR(sql);
		PHALCON_CONCAT_SVS(sql, "CREATE TABLE ", table, " (\n\t");
	}
	
	PHALCON_INIT_VAR(create_lines);
	array_init(create_lines);
	
	PHALCON_ALLOC_ZVAL_MM(r3);
	phalcon_array_fetch_string(&r3, definition, SL("columns"), PH_NOISY_CC);
	if (!phalcon_valid_foreach(r3 TSRMLS_CC)) {
		return;
	}
	
	ah0 = Z_ARRVAL_P(r3);
	zend_hash_internal_pointer_reset_ex(ah0, &hp0);
	fes_46b6_2:
		if(zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) != SUCCESS){
			goto fee_46b6_2;
		}
		
		PHALCON_INIT_VAR(column);
		ZVAL_ZVAL(column, *hd, 1, 0);
		PHALCON_INIT_VAR(r4);
		PHALCON_CALL_METHOD(r4, column, "getname", PH_NO_CHECK);
		PHALCON_INIT_VAR(r5);
		PHALCON_CALL_SELF_PARAMS_1(r5, this_ptr, "getcolumndefinition", column);
		PHALCON_INIT_VAR(column_line);
		PHALCON_CONCAT_SVSV(column_line, "`", r4, "` ", r5);
		
		PHALCON_INIT_VAR(r6);
		PHALCON_CALL_METHOD(r6, column, "isnotnull", PH_NO_CHECK);
		if (zend_is_true(r6)) {
			PHALCON_INIT_VAR(t0);
			ZVAL_STRING(t0, " NOT NULL", 1);
			phalcon_concat_self(&column_line, t0 TSRMLS_CC);
		}
		
		PHALCON_INIT_VAR(r7);
		PHALCON_CALL_METHOD(r7, column, "isautoincrement", PH_NO_CHECK);
		if (zend_is_true(r7)) {
			PHALCON_INIT_VAR(t1);
			ZVAL_STRING(t1, " AUTO_INCREMENT", 1);
			phalcon_concat_self(&column_line, t1 TSRMLS_CC);
		}
		
		phalcon_array_append(&create_lines, column_line, PH_SEPARATE TSRMLS_CC);
		zend_hash_move_forward_ex(ah0, &hp0);
		goto fes_46b6_2;
	fee_46b6_2:
	if(0){}
	
	eval_int = phalcon_array_isset_string(definition, SL("indexes")+1);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(r8);
		phalcon_array_fetch_string(&r8, definition, SL("indexes"), PH_NOISY_CC);
		if (!phalcon_valid_foreach(r8 TSRMLS_CC)) {
			return;
		}
		
		ah1 = Z_ARRVAL_P(r8);
		zend_hash_internal_pointer_reset_ex(ah1, &hp1);
		fes_46b6_3:
			if(zend_hash_get_current_data_ex(ah1, (void**) &hd, &hp1) != SUCCESS){
				goto fee_46b6_3;
			}
			
			PHALCON_INIT_VAR(index);
			ZVAL_ZVAL(index, *hd, 1, 0);
			PHALCON_INIT_VAR(index_name);
			PHALCON_CALL_METHOD(index_name, index, "getname", PH_NO_CHECK);
			
			PHALCON_INIT_VAR(r9);
			PHALCON_CALL_METHOD(r9, index, "getcolumns", PH_NO_CHECK);
			
			PHALCON_INIT_VAR(column_list);
			PHALCON_CALL_SELF_PARAMS_1(column_list, this_ptr, "getcolumnlist", r9);
			if (PHALCON_COMPARE_STRING(index_name, "PRIMARY")) {
				PHALCON_INIT_VAR(r10);
				PHALCON_CONCAT_SVS(r10, "PRIMARY KEY (", column_list, ")");
				phalcon_array_append(&create_lines, r10, PH_SEPARATE TSRMLS_CC);
			} else {
				PHALCON_INIT_VAR(r11);
				PHALCON_CONCAT_SVSVS(r11, "KEY `", index_name, "` (", column_list, ")");
				phalcon_array_append(&create_lines, r11, PH_SEPARATE TSRMLS_CC);
			}
			zend_hash_move_forward_ex(ah1, &hp1);
			goto fes_46b6_3;
		fee_46b6_3:
		if(0){}
		
	}
	
	eval_int = phalcon_array_isset_string(definition, SL("references")+1);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(r12);
		phalcon_array_fetch_string(&r12, definition, SL("references"), PH_NOISY_CC);
		if (!phalcon_valid_foreach(r12 TSRMLS_CC)) {
			return;
		}
		
		ah2 = Z_ARRVAL_P(r12);
		zend_hash_internal_pointer_reset_ex(ah2, &hp2);
		fes_46b6_4:
			if(zend_hash_get_current_data_ex(ah2, (void**) &hd, &hp2) != SUCCESS){
				goto fee_46b6_4;
			}
			
			PHALCON_INIT_VAR(reference);
			ZVAL_ZVAL(reference, *hd, 1, 0);
			PHALCON_INIT_VAR(r13);
			PHALCON_CALL_METHOD(r13, reference, "getname", PH_NO_CHECK);
			PHALCON_INIT_VAR(r14);
			PHALCON_CALL_METHOD(r14, reference, "getcolumns", PH_NO_CHECK);
			PHALCON_INIT_VAR(r15);
			PHALCON_CALL_SELF_PARAMS_1(r15, this_ptr, "getcolumnlist", r14);
			PHALCON_INIT_VAR(reference_sql);
			PHALCON_CONCAT_SVSV(reference_sql, "CONSTRAINT `", r13, "` FOREIGN KEY (", r15);
			
			PHALCON_INIT_VAR(r16);
			PHALCON_CALL_METHOD(r16, reference, "getreferencedtable", PH_NO_CHECK);
			
			PHALCON_INIT_VAR(r17);
			PHALCON_CALL_METHOD(r17, reference, "getreferencedcolumns", PH_NO_CHECK);
			
			PHALCON_INIT_VAR(r18);
			PHALCON_CALL_SELF_PARAMS_1(r18, this_ptr, "getcolumnlist", r17);
			
			PHALCON_INIT_VAR(r19);
			PHALCON_CONCAT_SVSVS(r19, ") REFERENCES `", r16, "`(", r18, ")");
			phalcon_concat_self(&reference_sql, r19 TSRMLS_CC);
			phalcon_array_append(&create_lines, reference_sql, PH_SEPARATE TSRMLS_CC);
			zend_hash_move_forward_ex(ah2, &hp2);
			goto fes_46b6_4;
		fee_46b6_4:
		if(0){}
		
	}
	
	PHALCON_INIT_VAR(c0);
	ZVAL_STRING(c0, ",\n\t", 1);
	
	PHALCON_ALLOC_ZVAL_MM(r20);
	phalcon_fast_join(r20, c0, create_lines TSRMLS_CC);
	
	PHALCON_ALLOC_ZVAL_MM(r21);
	PHALCON_CONCAT_VS(r21, r20, "\n)");
	phalcon_concat_self(&sql, r21 TSRMLS_CC);
	eval_int = phalcon_array_isset_string(definition, SL("options")+1);
	if (eval_int) {
		PHALCON_ALLOC_ZVAL_MM(r22);
		PHALCON_CALL_SELF_PARAMS_1(r22, this_ptr, "_gettableoptions", definition);
		PHALCON_ALLOC_ZVAL_MM(r23);
		PHALCON_CONCAT_SV(r23, " ", r22);
		phalcon_concat_self(&sql, r23 TSRMLS_CC);
	}
	
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to drop a table
 *
 * @param  string $tableName
 * @param  string $schemaName
 * @param  boolean $ifExists
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, dropTable){

	zval *table_name = NULL, *schema_name = NULL, *if_exists = NULL, *table = NULL;
	zval *r0 = NULL, *r1 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|z", &table_name, &schema_name, &if_exists) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!if_exists) {
		PHALCON_ALLOC_ZVAL_MM(if_exists);
		ZVAL_BOOL(if_exists, 1);
	}
	
	if (zend_is_true(schema_name)) {
		PHALCON_INIT_VAR(table);
		PHALCON_CONCAT_VSV(table, schema_name, ".", table_name);
	} else {
		PHALCON_CPY_WRT(table, table_name);
	}
	if (zend_is_true(if_exists)) {
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CONCAT_SV(r0, "DROP TABLE IF EXISTS ", table);
		
		RETURN_CTOR(r0);
	} else {
		PHALCON_ALLOC_ZVAL_MM(r1);
		PHALCON_CONCAT_SV(r1, "DROP TABLE ", table);
		
		RETURN_CTOR(r1);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Generates SQL checking for the existence of a schema.table
 *
 * @param string $tableName
 * @param string $schemaName
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, tableExists){

	zval *table_name = NULL, *schema_name = NULL;
	zval *r0 = NULL, *r1 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table_name, &schema_name) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema_name) {
		PHALCON_ALLOC_ZVAL_MM(schema_name);
		ZVAL_NULL(schema_name);
	}
	
	if (zend_is_true(schema_name)) {
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CONCAT_SVSVS(r0, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END FROM information_schema.tables WHERE table_schema = '", schema_name, "' AND table_name='", table_name, "'");
		
		RETURN_CTOR(r0);
	} else {
		PHALCON_ALLOC_ZVAL_MM(r1);
		PHALCON_CONCAT_SVS(r1, "SELECT CASE WHEN COUNT(*) > 0 THEN 1 ELSE 0 END FROM information_schema.tables WHERE table_schema = 'public' AND table_name='", table_name, "'");
		
		RETURN_CTOR(r1);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Generates a SQL describing a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, describeColumns){

	zval *table = NULL, *schema = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &schema) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema) {
		PHALCON_ALLOC_ZVAL_MM(schema);
		ZVAL_NULL(schema);
	}
	
	PHALCON_ALLOC_ZVAL_MM(r0);
	PHALCON_CONCAT_SVSVS(r0, "SELECT a.attname AS Field, format_type(a.atttypid, a.atttypmod) AS Type, CASE WHEN attnotnull=false THEN 'YES' ELSE 'NO' END AS Null, CASE WHEN (select cc.contype FROM pg_catalog.pg_constraint cc WHERE cc.conrelid = c.oid AND cc.conkey[1] = a.attnum)='p' THEN 'PRI' ELSE '' END AS Key, CASE WHEN t.typname LIKE '%int%' AND (SELECT column_default FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = '", table, "' AND column_name = a.attname) LIKE '%nextval%' THEN 'auto_increment' ELSE '' END AS Extra FROM pg_catalog.pg_Class c, pg_catalog.pg_attribute a, pg_catalog.pg_type t WHERE c.relname = '", table, "' AND c.oid = a.attrelid AND a.attnum > 0 AND c.relhaspkey = 't' AND t.oid = a.atttypid ORDER BY a.attnum");
	
	RETURN_CTOR(r0);
}

/**
 * List all tables on database
 *
 * @param       string $schemaName
 * @return      array
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, listTables){

	zval *schema_name = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &schema_name) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema_name) {
		PHALCON_ALLOC_ZVAL_MM(schema_name);
		ZVAL_NULL(schema_name);
	}
	
	PHALCON_MM_RESTORE();
	RETURN_STRING("SELECT c.relname AS table_name FROM pg_Class c, pg_user u WHERE c.relowner = u.usesysid AND c.relkind = 'r' AND NOT EXISTS (SELECT 1 FROM pg_views WHERE viewname = c.relname) AND c.relname !~ '^(pg_|sql_)' UNION SELECT c.relname AS table_name FROM pg_Class c WHERE c.relkind = 'r' AND NOT EXISTS (SELECT 1 FROM pg_views WHERE viewname = c.relname)  AND NOT EXISTS (SELECT 1 FROM pg_user WHERE usesysid = c.relowner)  AND c.relname !~ '^pg_' ORDER BY 1", 1);
}

/**
 * Generates SQL to query indexes on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, describeIndexes){

	zval *table = NULL, *schema = NULL, *sql = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &schema) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema) {
		PHALCON_ALLOC_ZVAL_MM(schema);
		ZVAL_NULL(schema);
	}
	
	PHALCON_INIT_VAR(sql);
	PHALCON_CONCAT_SVS(sql, "SELECT t.relname as table_name, i.relname as key_name, a.attname as column_name FROM pg_class t, pg_class i, pg_index ix, pg_attribute a WHERE t.oid = ix.indrelid AND i.oid = ix.indexrelid AND a.attrelid = t.oid AND a.attnum = ANY(ix.indkey) AND t.relkind = 'r' AND t.relname = '", table, "' ORDER BY t.relname, i.relname;");
	
	RETURN_CTOR(sql);
}

/**
 * Generates SQL to query foreign keys on a table
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, describeReferences){

	zval *table = NULL, *schema = NULL, *sql = NULL;
	zval *r0 = NULL, *r1 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &schema) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema) {
		PHALCON_ALLOC_ZVAL_MM(schema);
		ZVAL_NULL(schema);
	}
	
	PHALCON_INIT_VAR(sql);
	ZVAL_STRING(sql, "SELECT tc.table_name as TABLE_NAME, kcu.column_name as COLUMN_NAME, tc.constraint_name as CONSTRAINT_NAME, tc.table_catalog as REFERENCED_TABLE_SCHEMA, ccu.table_name AS REFERENCED_TABLE_NAME, ccu.column_name AS REFERENCED_COLUMN_NAME FROM information_schema.table_constraints AS tc JOIN information_schema.key_column_usage AS kcu ON tc.constraint_name = kcu.constraint_name JOIN information_schema.constraint_column_usage AS ccu ON ccu.constraint_name = tc.constraint_name WHERE constraint_type = 'FOREIGN KEY' AND ", 1);
	if (zend_is_true(schema)) {
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CONCAT_SVSVS(r0, "tc.table_catalog = '", schema, "' AND tc.table_name='", table, "'");
		phalcon_concat_self(&sql, r0 TSRMLS_CC);
	} else {
		PHALCON_ALLOC_ZVAL_MM(r1);
		PHALCON_CONCAT_SVS(r1, "tc.table_name='", table, "'");
		phalcon_concat_self(&sql, r1 TSRMLS_CC);
	}
	
	
	RETURN_CTOR(sql);
}

/**
 * Generates the SQL to describe the table creation options
 *
 * @param string $table
 * @param string $schema
 * @return string
 */
PHP_METHOD(Phalcon_Db_Dialect_Postgresql, tableOptions){

	zval *table = NULL, *schema = NULL, *sql = NULL;
	zval *r0 = NULL, *r1 = NULL;

	PHALCON_MM_GROW();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &table, &schema) == FAILURE) {
		PHALCON_MM_RESTORE();
		RETURN_NULL();
	}

	if (!schema) {
		PHALCON_ALLOC_ZVAL_MM(schema);
		ZVAL_NULL(schema);
	}
	
	PHALCON_INIT_VAR(sql);
	ZVAL_STRING(sql, "SELECT TABLES.TABLE_TYPE,TABLES.AUTO_INCREMENT,TABLES.ENGINE,TABLES.TABLE_COLLATION FROM INFORMATION_SCHEMA.TABLES WHERE ", 1);
	if (zend_is_true(schema)) {
		PHALCON_ALLOC_ZVAL_MM(r0);
		PHALCON_CONCAT_SVSVS(r0, "TABLES.TABLE_SCHEMA = \"", schema, "\" AND TABLES.TABLE_NAME = \"", table, "\"");
		phalcon_concat_self(&sql, r0 TSRMLS_CC);
	} else {
		PHALCON_ALLOC_ZVAL_MM(r1);
		PHALCON_CONCAT_SVS(r1, "TABLES.TABLE_NAME = \"", table, "\"");
		phalcon_concat_self(&sql, r1 TSRMLS_CC);
	}
	
	
	RETURN_CTOR(sql);
}

