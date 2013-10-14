function love.load()
	tekst = "a"
end

function love.draw()
    love.graphics.print(tekst, 400, 300)
end

function love.update(dt)
   tekst = tekst .. "a"
   end
end
