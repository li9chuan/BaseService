
login = {}

RegScriptEvent( "login", EventLogin )

local MISSION_ID = 1003000;

login.OnEvent = function( eventID, personID )

	if eventID==EventLogin then
		
		-- if MissionAccept(personID,1003)  then
			-- MissionComplete(personID,1003);
		-- end
		
		
		if GetMissionCompleteCount(personID,MISSION_ID) == 0 then
			AddHero(personID,1001001);
			AddHero(personID,1002001);
			AddHero(personID,1003001);
			AddCastle(personID,8000014);
			AddGuard(personID,13002039);
			AddGuard(personID,13001004);
			AddGuard(personID,13001005);
			-- SetMissionCompleteCount(personID,MISSION_ID,1);
		end
	end

end




