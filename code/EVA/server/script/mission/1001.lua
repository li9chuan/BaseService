mission_1001 = {}

RegMissionEvent( 1001, Event_Mission_Complete )

mission_1001.Accept = function(uid)
	local res = MISSION_ACCEPT_FAIL;
	if GetMissionCompleteCount(1001,uid) == 0 then
			res = MISSION_ACCEPT_SUCESS;
--		print("AddMissionTemp fail.  AddMissionTracker uid: mission_id:");
	else
		local comp_time = GetMissionCompleteTime(1001,uid);
		if GetCurrentDay1970(0) > GetCurrentDay1970(comp_time) then
			if SetMissionCompleteCount(uid,1001,1) then
				res = MISSION_ACCEPT_SUCESS;
			end
		end
	end
	return res;
end

mission_1001.Complete = function(uid)
	local res = MISSION_COMPLATE_FAIL;

	if GetMissionCompleteCount(1001,uid) == 0 then
		SetMissionCompleteCount(uid,1001,1);
		res = MISSION_COMPLATE_SUCESS;
	end
	return res;
end

mission_1001.OnEvent = function( eventID, personID, param1, param2 )

	if eventID==Event_Mission_Complete then

	end

end





