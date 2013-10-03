map.getPosition = function()
	return map.getX(), map.getY()
end

while true do
	actions.moveDown()
	actions.waitUntilFlush()
	
	actions.moveUp()
	actions.waitUntilFlush()
	
	local x,y = map.getPosition();
	local flags = map.lookAt(x,y-1);
	
	if bit32.band(flags, map.PLAYER) ~= 0 then
		print("PLAYER")
	elseif bit32.band(flags, map.BOMB) ~= 0 then
		print("BOMB")
	else
		print("NOTHING")
	end
	
end
