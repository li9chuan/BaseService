--  Ê×´ÎµÇÂ¼Àñ°ü

mission_1003 = {}

local MISSION_ID = 1003;
RegMissionID(MISSION_ID);

mission_1003.Accept = function(personID)
	if GetMissionCompleteCount(personID,MISSION_ID) == 0 then
		return MISSION_ACCEPT_SUCESS;
	else
		return MISSION_ACCEPT_FAIL;
	end
end

mission_1003.Complete = function(personID)
	if GetMissionCompleteCount(personID,MISSION_ID) == 0 then
		AddMoney(personID,100000);
		AddHero(personID,11800);
		AddItem(personID,1001);
		AddItem(personID,1002);
		SetMissionCompleteCount(personID,MISSION_ID,1);
		return MISSION_COMPLATE_SUCESS;
	end
	return MISSION_COMPLATE_FAIL;
end

mission_1003.OnEvent = function( eventID, personID, param1, param2 )
end





