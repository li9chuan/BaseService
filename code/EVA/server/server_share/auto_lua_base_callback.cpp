/*
** Lua binding: base
** Generated automatically by tolua++-1.0.92 on 02/09/18 17:59:39.
*/
//
//#ifndef __cplusplus
//#include "stdlib.h"
//#endif
//#include "string.h"
//
//#include "tolua++.h"
//
///* Exported function */
//TOLUA_API int  tolua_base_open (lua_State* tolua_S);
//
//#include "lua_base_function.h"
//#include <game_share/define_pro.pb.h>
//
///* function to register type */
//static void tolua_reg_types (lua_State* tolua_S)
//{
//}
//
///* function: GetCurrentDay1970 */
//#ifndef TOLUA_DISABLE_tolua_base_GetCurrentDay197000
//static int tolua_base_GetCurrentDay197000(lua_State* tolua_S)
//{
//#ifndef TOLUA_RELEASE
// tolua_Error tolua_err;
// if (
//     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
//     !tolua_isnoobj(tolua_S,2,&tolua_err)
// )
//  goto tolua_lerror;
// else
//#endif
// {
//  unsigned int time = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
//  {
//   int tolua_ret = (int)  GetCurrentDay1970(time);
//   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
//  }
// }
// return 1;
//#ifndef TOLUA_RELEASE
// tolua_lerror:
// tolua_error(tolua_S,"#ferror in function 'GetCurrentDay1970'.",&tolua_err);
// return 0;
//#endif
//}
//#endif //#ifndef TOLUA_DISABLE
//
///* function: LoadLuaFile */
//#ifndef TOLUA_DISABLE_tolua_base_LoadLuaFile00
//static int tolua_base_LoadLuaFile00(lua_State* tolua_S)
//{
//#ifndef TOLUA_RELEASE
// tolua_Error tolua_err;
// if (
//     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
//     !tolua_isnoobj(tolua_S,2,&tolua_err)
// )
//  goto tolua_lerror;
// else
//#endif
// {
//  const char* szFileName = ((const char*)  tolua_tostring(tolua_S,1,0));
//  {
//   LoadLuaFile(szFileName);
//  }
// }
// return 0;
//#ifndef TOLUA_RELEASE
// tolua_lerror:
// tolua_error(tolua_S,"#ferror in function 'LoadLuaFile'.",&tolua_err);
// return 0;
//#endif
//}
//#endif //#ifndef TOLUA_DISABLE
//
///* function: RegScriptEvent */
//#ifndef TOLUA_DISABLE_tolua_base_RegScriptEvent00
//static int tolua_base_RegScriptEvent00(lua_State* tolua_S)
//{
//#ifndef TOLUA_RELEASE
// tolua_Error tolua_err;
// if (
//     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
//     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
//     !tolua_isnoobj(tolua_S,3,&tolua_err)
// )
//  goto tolua_lerror;
// else
//#endif
// {
//  const char* script_scope = ((const char*)  tolua_tostring(tolua_S,1,0));
//  int event_id = ((int)  tolua_tonumber(tolua_S,2,0));
//  {
//   RegScriptEvent(script_scope,event_id);
//  }
// }
// return 0;
//#ifndef TOLUA_RELEASE
// tolua_lerror:
// tolua_error(tolua_S,"#ferror in function 'RegScriptEvent'.",&tolua_err);
// return 0;
//#endif
//}
//#endif //#ifndef TOLUA_DISABLE
//
///* Open function */
//TOLUA_API int tolua_base_open (lua_State* tolua_S)
//{
// tolua_open(tolua_S);
// tolua_reg_types(tolua_S);
// tolua_module(tolua_S,NULL,0);
// tolua_beginmodule(tolua_S,NULL);
//  tolua_function(tolua_S,"GetCurrentDay1970",tolua_base_GetCurrentDay197000);
//  tolua_function(tolua_S,"LoadLuaFile",tolua_base_LoadLuaFile00);
//  tolua_function(tolua_S,"RegScriptEvent",tolua_base_RegScriptEvent00);
//  tolua_constant(tolua_S,"EventInvalid",EventInvalid);
//  tolua_constant(tolua_S,"EventEnterGame",EventEnterGame);
//  tolua_constant(tolua_S,"EventPlayerUp",EventPlayerUp);
//  tolua_constant(tolua_S,"EventJoinRoom",EventJoinRoom);
//  tolua_constant(tolua_S,"EventLogin",EventLogin);
//  tolua_constant(tolua_S,"EventBlackJackWin",EventBlackJackWin);
//  tolua_constant(tolua_S,"EventBlackJackPush",EventBlackJackPush);
//  tolua_constant(tolua_S,"EventBlackJackLose",EventBlackJackLose);
//  tolua_constant(tolua_S,"EventBlackJackBust",EventBlackJackBust);
//  tolua_constant(tolua_S,"EventBlackJackSplit",EventBlackJackSplit);
//  tolua_constant(tolua_S,"EventBlackJackDouble",EventBlackJackDouble);
//  tolua_constant(tolua_S,"EventBlackJackInsurance",EventBlackJackInsurance);
//  tolua_constant(tolua_S,"EventChalBJTopRank",EventChalBJTopRank);
//  tolua_constant(tolua_S,"EventBlackJackAddMoney",EventBlackJackAddMoney);
//  tolua_constant(tolua_S,"EventRanking",EventRanking);
//  tolua_constant(tolua_S,"EventTotalPay",EventTotalPay);
//  tolua_constant(tolua_S,"EventInviteFight",EventInviteFight);
//  tolua_constant(tolua_S,"EventAddFriend",EventAddFriend);
//  tolua_constant(tolua_S,"EventAddDonate",EventAddDonate);
//  tolua_constant(tolua_S,"EventAddBind",EventAddBind);
//  tolua_constant(tolua_S,"EventAddRMB",EventAddRMB);
//  tolua_constant(tolua_S,"INVALID_TYPE",INVALID_TYPE);
//  tolua_constant(tolua_S,"ACCOUNT_LOGGED",ACCOUNT_LOGGED);
//  tolua_constant(tolua_S,"SERVER_FULL",SERVER_FULL);
//  tolua_constant(tolua_S,"SERVER_NOT_OPEN",SERVER_NOT_OPEN);
//  tolua_constant(tolua_S,"TEXT_SUCESS",TEXT_SUCESS);
//  tolua_constant(tolua_S,"TEXT_FAIL",TEXT_FAIL);
//  tolua_constant(tolua_S,"PWD_ERROR",PWD_ERROR);
//  tolua_constant(tolua_S,"PLAYER_ONLINE_TO_FES",PLAYER_ONLINE_TO_FES);
//  tolua_constant(tolua_S,"PLAYER_EXISTS",PLAYER_EXISTS);
//  tolua_constant(tolua_S,"PLAYER_RELOAD",PLAYER_RELOAD);
//  tolua_constant(tolua_S,"PLAYER_REMOVE",PLAYER_REMOVE);
//  tolua_constant(tolua_S,"GET_REWARD_TIME",GET_REWARD_TIME);
//  tolua_constant(tolua_S,"ACCOUNT_ERROR",ACCOUNT_ERROR);
//  tolua_constant(tolua_S,"NOT_FOUND_ACCOUNT",NOT_FOUND_ACCOUNT);
//  tolua_constant(tolua_S,"NOT_FOUND_PLAYER",NOT_FOUND_PLAYER);
//  tolua_constant(tolua_S,"SUCESS",SUCESS);
//  tolua_constant(tolua_S,"NO_AUTH_TYPE",NO_AUTH_TYPE);
//  tolua_constant(tolua_S,"CONFIG_NOT_FOUND",CONFIG_NOT_FOUND);
//  tolua_constant(tolua_S,"NOT_ENOUGH_MONEY",NOT_ENOUGH_MONEY);
//  tolua_constant(tolua_S,"NOT_ENOUGH_RMB",NOT_ENOUGH_RMB);
//  tolua_constant(tolua_S,"MAIL_NOT_FIND",MAIL_NOT_FIND);
//  tolua_constant(tolua_S,"CMAIL_NOTNEW_MAIL",CMAIL_NOTNEW_MAIL);
//  tolua_constant(tolua_S,"NOT_ENOUGH_COIN",NOT_ENOUGH_COIN);
//  tolua_constant(tolua_S,"NOT_FOUND_REC_CHAMPIONSHIP",NOT_FOUND_REC_CHAMPIONSHIP);
//  tolua_constant(tolua_S,"SYSTEM_DATED_MAIL",SYSTEM_DATED_MAIL);
//  tolua_constant(tolua_S,"COMMON_DATED_MAIL",COMMON_DATED_MAIL);
//  tolua_constant(tolua_S,"COMMON_EXIST_REWARD",COMMON_EXIST_REWARD);
//  tolua_constant(tolua_S,"FRIEND_APPLY_FAILE",FRIEND_APPLY_FAILE);
//  tolua_constant(tolua_S,"FRIEND_APPLY_SUCCEED",FRIEND_APPLY_SUCCEED);
//  tolua_constant(tolua_S,"TARGET_NO_EXIST",TARGET_NO_EXIST);
//  tolua_constant(tolua_S,"FRIEND_RELATION",FRIEND_RELATION);
//  tolua_constant(tolua_S,"FRIEND_ADD_FAILE",FRIEND_ADD_FAILE);
//  tolua_constant(tolua_S,"FRIEND_REWARD_FAILE",FRIEND_REWARD_FAILE);
//  tolua_constant(tolua_S,"FRIEND_REWARD_SUCCEED",FRIEND_REWARD_SUCCEED);
//  tolua_constant(tolua_S,"COMMON_MAIL_MAX_COUNT",COMMON_MAIL_MAX_COUNT);
//  tolua_constant(tolua_S,"ROOM_TYPE_IS_FULL",ROOM_TYPE_IS_FULL);
//  tolua_constant(tolua_S,"JOIN_ROOM_FAILE",JOIN_ROOM_FAILE);
//  tolua_constant(tolua_S,"TIPS_COUNT_OUT_OF_BOUNDS",TIPS_COUNT_OUT_OF_BOUNDS);
//  tolua_constant(tolua_S,"WATCH_THE_ADS_FAIL",WATCH_THE_ADS_FAIL);
//  tolua_constant(tolua_S,"REWARD_FRIEND_FAILE",REWARD_FRIEND_FAILE);
//  tolua_constant(tolua_S,"ROOM_TYPE_FAILE",ROOM_TYPE_FAILE);
//  tolua_constant(tolua_S,"NEED_RELOGIN",NEED_RELOGIN);
//  tolua_constant(tolua_S,"GAIN_MAIL_COMMON_FAILE",GAIN_MAIL_COMMON_FAILE);
//  tolua_constant(tolua_S,"GAIN_MAIL_SYSTEM_FAILE",GAIN_MAIL_SYSTEM_FAILE);
//  tolua_constant(tolua_S,"REFRESH_MONEY",REFRESH_MONEY);
//  tolua_constant(tolua_S,"ROOM_SEAT_FULL",ROOM_SEAT_FULL);
//  tolua_constant(tolua_S,"ALREADY_LIKE",ALREADY_LIKE);
//  tolua_constant(tolua_S,"BONUS_GAME_ERROR",BONUS_GAME_ERROR);
//  tolua_constant(tolua_S,"NOT_FOUND_ROOM",NOT_FOUND_ROOM);
//  tolua_constant(tolua_S,"FRIEND_NO_ONLINE",FRIEND_NO_ONLINE);
//  tolua_constant(tolua_S,"INVITE_FRIEND_SUCCEED",INVITE_FRIEND_SUCCEED);
//  tolua_constant(tolua_S,"TP_APPLY_NO_MONEY",TP_APPLY_NO_MONEY);
//  tolua_constant(tolua_S,"TP_APPLY_REGISTERED",TP_APPLY_REGISTERED);
//  tolua_constant(tolua_S,"TP_ENTER_CHALL_FAILE",TP_ENTER_CHALL_FAILE);
//  tolua_constant(tolua_S,"TP_APPLY_GAME_PAST",TP_APPLY_GAME_PAST);
//  tolua_constant(tolua_S,"TARGET_PLAYER_REC_GIFT_MAX",TARGET_PLAYER_REC_GIFT_MAX);
//  tolua_constant(tolua_S,"SEND_MONEY_SUCESS",SEND_MONEY_SUCESS);
//  tolua_constant(tolua_S,"SEND_MONEY_LIMIT",SEND_MONEY_LIMIT);
//  tolua_constant(tolua_S,"BUY_BACK_LIMIT",BUY_BACK_LIMIT);
//  tolua_constant(tolua_S,"BLACK_LIST_RELATION",BLACK_LIST_RELATION);
//  tolua_constant(tolua_S,"BLACK_LIST_NOT_IN",BLACK_LIST_NOT_IN);
//  tolua_constant(tolua_S,"RENAME_TIME_LIMIT_FAILE",RENAME_TIME_LIMIT_FAILE);
//  tolua_constant(tolua_S,"SELF_FRIEND_LIST_FULL",SELF_FRIEND_LIST_FULL);
//  tolua_constant(tolua_S,"TARGET_FRIEND_LIST_FULL",TARGET_FRIEND_LIST_FULL);
//  tolua_constant(tolua_S,"SAFE_BOX_PWD_ERROR",SAFE_BOX_PWD_ERROR);
//  tolua_constant(tolua_S,"SAFE_BOX_PWD_IS_SET",SAFE_BOX_PWD_IS_SET);
//  tolua_constant(tolua_S,"SAFE_BOX_IS_OPEN",SAFE_BOX_IS_OPEN);
//  tolua_constant(tolua_S,"SAFE_BOX_NEED_OPEN",SAFE_BOX_NEED_OPEN);
//  tolua_constant(tolua_S,"NOT_ENOUGH_ITEM",NOT_ENOUGH_ITEM);
//  tolua_constant(tolua_S,"LIMITUSE_FAILED",LIMITUSE_FAILED);
//  tolua_constant(tolua_S,"MOBILE_BIND_SUCESS",MOBILE_BIND_SUCESS);
//  tolua_constant(tolua_S,"MOBILE_BIND_ERROR",MOBILE_BIND_ERROR);
//  tolua_constant(tolua_S,"NO_FRIEND_RELATION",NO_FRIEND_RELATION);
//  tolua_constant(tolua_S,"ADD_ITEM_SUCCEED",ADD_ITEM_SUCCEED);
//  tolua_constant(tolua_S,"ADD_ITEM_FAIL",ADD_ITEM_FAIL);
//  tolua_constant(tolua_S,"SEND_MAIL_NOT_FIND_ITEM",SEND_MAIL_NOT_FIND_ITEM);
//  tolua_constant(tolua_S,"SEND_MAIL_ITEM_COUNT_FAIL",SEND_MAIL_ITEM_COUNT_FAIL);
//  tolua_constant(tolua_S,"USE_ITEM_SUCESS",USE_ITEM_SUCESS);
//  tolua_constant(tolua_S,"SCORE_CLEAR_POSITIVE",SCORE_CLEAR_POSITIVE);
//  tolua_constant(tolua_S,"BUY_ITEM_MONEY_NOT_ENOUGH",BUY_ITEM_MONEY_NOT_ENOUGH);
//  tolua_constant(tolua_S,"BUY_ITEM_RMB_NOT_ENOUGH",BUY_ITEM_RMB_NOT_ENOUGH);
//  tolua_constant(tolua_S,"BUY_ITEM_VIP_LEVEL_NOT_ENOUGH",BUY_ITEM_VIP_LEVEL_NOT_ENOUGH);
//  tolua_constant(tolua_S,"FREEZE_PLAYER",FREEZE_PLAYER);
//  tolua_constant(tolua_S,"NOT_FOUND_ITEM",NOT_FOUND_ITEM);
//  tolua_constant(tolua_S,"NOT_TIP_ITEM_OUT_ROOM",NOT_TIP_ITEM_OUT_ROOM);
//  tolua_constant(tolua_S,"SYNC_ITEM_CRC_EQUAL",SYNC_ITEM_CRC_EQUAL);
//  tolua_constant(tolua_S,"FRIEND_APPLY_REQUEST",FRIEND_APPLY_REQUEST);
//  tolua_constant(tolua_S,"ICON_ALREADY_ACTIVATE",ICON_ALREADY_ACTIVATE);
//  tolua_constant(tolua_S,"ICON_ALREADY_UNLOCK",ICON_ALREADY_UNLOCK);
//  tolua_constant(tolua_S,"ICON_NOT_ACTIVATE_NOT_UNCLOCK",ICON_NOT_ACTIVATE_NOT_UNCLOCK);
//  tolua_constant(tolua_S,"CURR_UNCLOCK_ICON_ID",CURR_UNCLOCK_ICON_ID);
//  tolua_constant(tolua_S,"QUICK_UNCLOCK_ICON_ID_FAILE",QUICK_UNCLOCK_ICON_ID_FAILE);
//  tolua_constant(tolua_S,"MOBILE_BIND_TIMEOUT",MOBILE_BIND_TIMEOUT);
//  tolua_constant(tolua_S,"SEND_ITEM_SUCCESS",SEND_ITEM_SUCCESS);
//  tolua_constant(tolua_S,"USER_READY_NOT_MONEY",USER_READY_NOT_MONEY);
//  tolua_constant(tolua_S,"SAFE_BOX_PWD_SET_ERROR",SAFE_BOX_PWD_SET_ERROR);
//  tolua_constant(tolua_S,"EXIST_FRIEND_APPLY",EXIST_FRIEND_APPLY);
//  tolua_constant(tolua_S,"CREATE_PRIVATE_ROOM_FAILE",CREATE_PRIVATE_ROOM_FAILE);
//  tolua_constant(tolua_S,"HAVE_PRIVATE_ROOM",HAVE_PRIVATE_ROOM);
//  tolua_constant(tolua_S,"NEED_ENTER_SELF_ROOM",NEED_ENTER_SELF_ROOM);
//  tolua_constant(tolua_S,"PRIVATE_ROOM_NOTEXIST",PRIVATE_ROOM_NOTEXIST);
//  tolua_constant(tolua_S,"ENTER_PRIVATE_ROOM_FAILE",ENTER_PRIVATE_ROOM_FAILE);
//  tolua_constant(tolua_S,"JOIN_PRIVATE_ROOM_FAILE",JOIN_PRIVATE_ROOM_FAILE);
//  tolua_constant(tolua_S,"PRIVATE_ROOM_CONSUME_FALIE",PRIVATE_ROOM_CONSUME_FALIE);
//  tolua_constant(tolua_S,"HORSE_ID_ERROR",HORSE_ID_ERROR);
//  tolua_constant(tolua_S,"PRIVATE_ROOM_ITEM_FAILE",PRIVATE_ROOM_ITEM_FAILE);
//  tolua_constant(tolua_S,"RELIEVE_ROOM_NOT_STARTGAME",RELIEVE_ROOM_NOT_STARTGAME);
//  tolua_constant(tolua_S,"WEAPON_LEVEL_ERROR",WEAPON_LEVEL_ERROR);
//  tolua_constant(tolua_S,"BULLET_ALREADY_UNLOCK",BULLET_ALREADY_UNLOCK);
//  tolua_constant(tolua_S,"SEND_ENVELOPE_MONEY_LIMIT",SEND_ENVELOPE_MONEY_LIMIT);
//  tolua_constant(tolua_S,"NO_MORE_ENVELOPE",NO_MORE_ENVELOPE);
//  tolua_constant(tolua_S,"ALREADY_GOT_THIS_ENVELOPE",ALREADY_GOT_THIS_ENVELOPE);
//  tolua_constant(tolua_S,"NOT_ENOUGH_COUPON",NOT_ENOUGH_COUPON);
//  tolua_constant(tolua_S,"NIUNIU_JOINROOM_FAIL",NIUNIU_JOINROOM_FAIL);
//  tolua_constant(tolua_S,"NIUNIU_STARTGAME_FAIL",NIUNIU_STARTGAME_FAIL);
//  tolua_constant(tolua_S,"RUNFAST_OUTCARD_TYPE_ERROR",RUNFAST_OUTCARD_TYPE_ERROR);
//  tolua_constant(tolua_S,"RUNFAST_OUTCARD_NOTHEISAN",RUNFAST_OUTCARD_NOTHEISAN);
//  tolua_constant(tolua_S,"RUNFAST_OUTCARD_GUANBUZHU",RUNFAST_OUTCARD_GUANBUZHU);
//  tolua_constant(tolua_S,"OUTCARD_INVALID",OUTCARD_INVALID);
//  tolua_constant(tolua_S,"GIFT_CARD_INVALID",GIFT_CARD_INVALID);
//  tolua_constant(tolua_S,"RECV_GIFT_CARD_INVALID",RECV_GIFT_CARD_INVALID);
//  tolua_constant(tolua_S,"GUANDAN_CANNOT_PASS_CARD",GUANDAN_CANNOT_PASS_CARD);
//  tolua_constant(tolua_S,"MAHJONG_MODA_LIMIT_OUT_CARD",MAHJONG_MODA_LIMIT_OUT_CARD);
//  tolua_constant(tolua_S,"MAHJONG_VERSION_FAILE",MAHJONG_VERSION_FAILE);
//  tolua_constant(tolua_S,"RUNFAST_OUTCARD_FAILE",RUNFAST_OUTCARD_FAILE);
//  tolua_constant(tolua_S,"BIND_PHONE_ERROR",BIND_PHONE_ERROR);
//  tolua_constant(tolua_S,"RFQ_JNPL_MAX",RFQ_JNPL_MAX);
//  tolua_constant(tolua_S,"RFQ_NO_FIND",RFQ_NO_FIND);
//  tolua_constant(tolua_S,"RFQ_NO_ACTIME",RFQ_NO_ACTIME);
//  tolua_constant(tolua_S,"RFQ_AC_MAX",RFQ_AC_MAX);
//  tolua_constant(tolua_S,"RFQ_JN_MAX",RFQ_JN_MAX);
//  tolua_constant(tolua_S,"RFQ_AC_ITEM",RFQ_AC_ITEM);
//  tolua_constant(tolua_S,"RFQ_JN_ITEM",RFQ_JN_ITEM);
//  tolua_constant(tolua_S,"RFQ_AC_CANCEL",RFQ_AC_CANCEL);
//  tolua_constant(tolua_S,"RFQ_JN_CANCEL",RFQ_JN_CANCEL);
//  tolua_constant(tolua_S,"RFQ_NO_JNTIME",RFQ_NO_JNTIME);
//  tolua_constant(tolua_S,"RFQ_JN_RE",RFQ_JN_RE);
//  tolua_constant(tolua_S,"RFQ_JN_ER",RFQ_JN_ER);
//  tolua_constant(tolua_S,"RFQ_ERR_STATE",RFQ_ERR_STATE);
//  tolua_constant(tolua_S,"RFQ_ST_ER",RFQ_ST_ER);
//  tolua_constant(tolua_S,"RFQ_RF_UMT",RFQ_RF_UMT);
//  tolua_constant(tolua_S,"RF_UN_TR",RF_UN_TR);
//  tolua_constant(tolua_S,"RF_ST_ROOM",RF_ST_ROOM);
//  tolua_constant(tolua_S,"RF_ST_NOIDLE",RF_ST_NOIDLE);
//  tolua_constant(tolua_S,"RFQ_TIME_ACIN",RFQ_TIME_ACIN);
//  tolua_constant(tolua_S,"RFQ_TIME_JNIN",RFQ_TIME_JNIN);
//  tolua_constant(tolua_S,"RFQ_ACTION_ER",RFQ_ACTION_ER);
//  tolua_constant(tolua_S,"RFQ_TIME_ER",RFQ_TIME_ER);
//  tolua_constant(tolua_S,"DDZ_OUTCARD_TYPE_ERROR",DDZ_OUTCARD_TYPE_ERROR);
//  tolua_constant(tolua_S,"DDZ_OUTCARD_GUANBUZHU",DDZ_OUTCARD_GUANBUZHU);
//  tolua_constant(tolua_S,"DDZ_OUTCARD_FAILE",DDZ_OUTCARD_FAILE);
//  tolua_constant(tolua_S,"BUY_ITEM_JIFEN_NOT_ENOUGH",BUY_ITEM_JIFEN_NOT_ENOUGH);
//  tolua_constant(tolua_S,"PF_CHANGE_SUCCEED",PF_CHANGE_SUCCEED);
//  tolua_constant(tolua_S,"PF_CHANGE_FAILED",PF_CHANGE_FAILED);
//  tolua_constant(tolua_S,"BUY_LIMIT_COUNT",BUY_LIMIT_COUNT);
//  tolua_constant(tolua_S,"RFQ_NO_OPEN",RFQ_NO_OPEN);
//  tolua_constant(tolua_S,"BIND_IDCARD_ERROR",BIND_IDCARD_ERROR);
//  tolua_constant(tolua_S,"BIND_IDCARD_TIMEOUT",BIND_IDCARD_TIMEOUT);
//  tolua_constant(tolua_S,"BIND_IDCARD_SUCCEED",BIND_IDCARD_SUCCEED);
//  tolua_constant(tolua_S,"RF_JNNO_BIND",RF_JNNO_BIND);
//  tolua_constant(tolua_S,"RF_QY_REG",RF_QY_REG);
//  tolua_constant(tolua_S,"RF_QYBC_REG",RF_QYBC_REG);
//  tolua_constant(tolua_S,"ALREADY_BIND",ALREADY_BIND);
//  tolua_constant(tolua_S,"TARGET_BIND_LIST_FULL",TARGET_BIND_LIST_FULL);
//  tolua_constant(tolua_S,"DST_ID_ERROR",DST_ID_ERROR);
//  tolua_constant(tolua_S,"DST_STATE_ERROR",DST_STATE_ERROR);
//  tolua_constant(tolua_S,"DST_PLAY_COUNT_ERROR",DST_PLAY_COUNT_ERROR);
//  tolua_constant(tolua_S,"SELF_BIRTH_TIME_OLD",SELF_BIRTH_TIME_OLD);
//  tolua_constant(tolua_S,"SELF_PLAY_COUNT_ERROR",SELF_PLAY_COUNT_ERROR);
//  tolua_constant(tolua_S,"SELF_BIND_STATE_ERROR",SELF_BIND_STATE_ERROR);
//  tolua_constant(tolua_S,"APP_NAME_DIFFERENT",APP_NAME_DIFFERENT);
//  tolua_constant(tolua_S,"BIND_PROXY_FAILE",BIND_PROXY_FAILE);
//  tolua_constant(tolua_S,"BIND_PROXY_ECHO",BIND_PROXY_ECHO);
//  tolua_constant(tolua_S,"BIND_PROXY_ING",BIND_PROXY_ING);
//  tolua_constant(tolua_S,"REQUEST_BIND_PROXY",REQUEST_BIND_PROXY);
//  tolua_constant(tolua_S,"BIND_BANK_ERROR",BIND_BANK_ERROR);
//  tolua_constant(tolua_S,"BIND_BANK_TIMEOUT",BIND_BANK_TIMEOUT);
//  tolua_constant(tolua_S,"BIND_BANK_SUCCEED",BIND_BANK_SUCCEED);
//  tolua_constant(tolua_S,"TIP_ITEM_NOT",TIP_ITEM_NOT);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_NULL",FRIEND_TISSUE_NULL);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_NOT_JOIN",FRIEND_TISSUE_NOT_JOIN);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_ID_ERR",FRIEND_TISSUE_ID_ERR);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_NOT_OWNER",FRIEND_TISSUE_NOT_OWNER);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_JOIN_OVER_COUNT",FRIEND_TISSUE_JOIN_OVER_COUNT);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_REMOVE_ID_SAME",FRIEND_TISSUE_REMOVE_ID_SAME);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_ALLREADY_IN",FRIEND_TISSUE_ALLREADY_IN);
//  tolua_constant(tolua_S,"FRIEND_TISSUE_AUTHORISE_FAIL",FRIEND_TISSUE_AUTHORISE_FAIL);
//  tolua_constant(tolua_S,"VIP_TISSUE_CREATE_COST_ERR",VIP_TISSUE_CREATE_COST_ERR);
//  tolua_constant(tolua_S,"CREATE_TISSUE_SUB_ITEM_FAIL",CREATE_TISSUE_SUB_ITEM_FAIL);
//  tolua_constant(tolua_S,"VIP_FRIEND_TISSUE_OVER_COUNT",VIP_FRIEND_TISSUE_OVER_COUNT);
//  tolua_constant(tolua_S,"NOR_FRIEND_TISSUE_OVER_COUNT",NOR_FRIEND_TISSUE_OVER_COUNT);
//  tolua_constant(tolua_S,"RFQ_AC_TIMELIMIT",RFQ_AC_TIMELIMIT);
//  tolua_constant(tolua_S,"RFQ_QY_ERTYPE",RFQ_QY_ERTYPE);
//  tolua_constant(tolua_S,"REQUEST_QY_SCORE",REQUEST_QY_SCORE);
//  tolua_constant(tolua_S,"REQUEST_QY_MONEY",REQUEST_QY_MONEY);
//  tolua_constant(tolua_S,"REQUEST_QY_DAER",REQUEST_QY_DAER);
//  tolua_constant(tolua_S,"REQUEST_HTQY_SCORE",REQUEST_HTQY_SCORE);
//  tolua_constant(tolua_S,"REQUEST_HTQY_MONEY",REQUEST_HTQY_MONEY);
//  tolua_constant(tolua_S,"PROXY_ID_DIFFERENT",PROXY_ID_DIFFERENT);
//  tolua_constant(tolua_S,"NOT_PROXY_ID",NOT_PROXY_ID);
//  tolua_constant(tolua_S,"SICHUAN_HUANSANZHANG_FAIL",SICHUAN_HUANSANZHANG_FAIL);
//  tolua_constant(tolua_S,"SHOP_BUY_ERROR",SHOP_BUY_ERROR);
//  tolua_constant(tolua_S,"RELIEVEROOM_FAILE_BYSTATE",RELIEVEROOM_FAILE_BYSTATE);
//  tolua_constant(tolua_S,"RECORD_NOT_EXIST",RECORD_NOT_EXIST);
//  tolua_constant(tolua_S,"GANDENGYAN_JOINROOM_FAIL",GANDENGYAN_JOINROOM_FAIL);
//  tolua_constant(tolua_S,"GANDENGYAN_STARTGAME_FAIL",GANDENGYAN_STARTGAME_FAIL);
//  tolua_constant(tolua_S,"UNBIND_PROXY_FAILE",UNBIND_PROXY_FAILE);
//  tolua_constant(tolua_S,"UNBIND_PROXY_NONE",UNBIND_PROXY_NONE);
//  tolua_constant(tolua_S,"UNBIND_PROXY_ING",UNBIND_PROXY_ING);
//  tolua_constant(tolua_S,"UNBIND_PROXY_SUCCEED",UNBIND_PROXY_SUCCEED);
//  tolua_constant(tolua_S,"CHESS_REGRETCHESS_REFUSE",CHESS_REGRETCHESS_REFUSE);
//  tolua_constant(tolua_S,"CHESS_ASKPEACE_REFUSE",CHESS_ASKPEACE_REFUSE);
//  tolua_constant(tolua_S,"CHESS_MOVECHESS_FAILED",CHESS_MOVECHESS_FAILED);
// tolua_endmodule(tolua_S);
// return 1;
//}
//
//
//#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
// TOLUA_API int luaopen_base (lua_State* tolua_S) {
// return tolua_base_open(tolua_S);
//};
//#endif
//
//