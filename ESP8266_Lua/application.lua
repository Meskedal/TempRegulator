-- server listens on 80, if data received, print data to console and send "hello world" back to caller
-- 30s time out for a inactive client
 -- Start a simple http server
 -- Start a simple http server
 -- Start a simple http server
gpio.mode(4,gpio.OUTPUT)
srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
  conn:on("receive",function(conn,payload)
    gpio.write(4, gpio.LOW)
    print(payload)
    conn:send("<h1> Hello, ("..data..") </h1>")
  end)
  gpio.write(4, gpio.HIGH)
  conn:on("sent",function(conn) conn:close() end)
end)

uart.on("data", 4,
  function(data)
    print("receive from uart:", data)
    if data=="quit" then
      uart.on("data") -- unregister callback function
    end
end, 0)