#pragma once

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

#include <iostream>
#include <stdio.h>

void test_localtime();
google::protobuf::Message* createMessage(const std::string& typeName);

void test_protobuf();

void test_id_gen();

void test_string_cow();

void test_crash();

void test_eval_expr();

void test_timer();

void test_zlib(std::string& str);

void test_dupstar();

void test_google_hash();

void test_udp();

void test_i18n();

//void test_ranking();
void test_ranking_min();

void test_hashset();
void test_set();
void test_callback();

int test_refresh_time( int start_time/*开启时间 单位:秒*/ , int takt_time/*间隔时间 单位:秒*/ );





