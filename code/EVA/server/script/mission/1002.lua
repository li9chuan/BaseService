mission_1002 = {}

local MISSION_ID = 1002;
RegMissionEvent( MISSION_ID, Event_Battle_Victory )

mission_1002.Accept = function(uid)
	return MISSION_ACCEPT_SUCESS;
end

mission_1002.Complete = function(uid)
	return MISSION_COMPLATE_SUCESS;
end

mission_1002.OnEvent = function( eventID, personID, jgtmID, param2 )

	if eventID==Event_Battle_Victory and jgtmID==1 then
		MissionComplete(personID,MISSION_ID);
	end

end





