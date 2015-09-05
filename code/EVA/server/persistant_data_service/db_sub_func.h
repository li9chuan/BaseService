/********************************************************************
    created:    2014/08/28
    created:    28:8:2014   11:30
    filename:   E:\MT\trunk\code\EVA\server\persistant_data_service\db_sub_func.h
    file path:  E:\MT\trunk\code\EVA\server\persistant_data_service
    file base:  db_sub_func
    file ext:   h
    author:     ChenKang

    purpose:    数据库线程执行函数;
*********************************************************************/

#ifndef PDS_DB_SUB_FUNC_H
#define PDS_DB_SUB_FUNC_H

/* t_playerinfo */
// query;
void db_sub_query_playerinfo( void* data );
// insert;
void db_sub_insert_playerinfo( void* data );
// update;
void db_sub_update_playerinfo( void* data );
// delete;
void db_sub_delete_playerinfo( void* data );


#endif // PDS_DB_SUB_FUNC_H
