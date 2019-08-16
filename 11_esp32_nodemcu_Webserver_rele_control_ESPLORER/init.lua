station_cfg={}
station_cfg.ssid="ESP32"
station_cfg.pwd="12345678"


wifi.setmode(wifi.STATION)
wifi.sta.config(station_cfg)
cfgip = {
    ip="192.168.1.112",
    netmask="255.255.255.0",
    gateway="192.168.1.1"
}
wifi.sta.setip(cfgip)
wifi.sta.autoconnect(1)

LEDpin = 4
gpio.mode(LEDpin, gpio.OUTPUT)

tmr.alarm(0, 1000, 1, function()
    if wifi.sta.getip() == nil then
        print("Conectando a rede...\n")
    else
        ip=wifi.sta.getip()
        print("IP: ", ip)
        tmr.stop(0)
    end
end)
server = net.createServer(net.TCP)
if server then
  server:listen(80, function(conn)
  conn:on("receive", receiver)
  end)
end

function SendHTML(sck, led)
   htmlstring = "HTTP/1.1 200 OK"
   htmlstring = "Content-Type: text/html"
   htmlstring = "<!DOCTYPE html>\r\n"
   htmlstring = htmlstring.."<html>\r\n"
   htmlstring = htmlstring.."<head>\r\n"
   htmlstring = htmlstring.."<meta charset=\"utf-8\">\r\n"
   htmlstring = htmlstring.."<style>html { font-family: Helvetica; background-color: #77878A; display: inline-block; margin: 0px auto; text-align: center;}</style>\r\n"           
   htmlstring = htmlstring.."<title>Controle de Lampadas</title>\r\n"
   htmlstring = htmlstring.."</head>\r\n"
   htmlstring = htmlstring.."<body>\r\n"
   htmlstring = htmlstring.."<h1>Controle de Lampadas</h1>\r\n"
   htmlstring = htmlstring.."<p>Clique para acender/apagar a lampada.</p>\r\n"
   htmlstring = htmlstring.."<form method=\"get\">\r\n"
  if (led)  then
   htmlstring = htmlstring.."<input type=\"button\" style=\"width:100px; height:100px; border-radius: 50%; background-color:#0192BA;\" value=\"LIGAR\" onclick=\"window.location.href='/ledoff'\">\r\n"
  else
   htmlstring = htmlstring.."<input type=\"button\" style=\"width:100px; height:100px; border-radius: 50%; background-color:#008A52;\" value=\"DESLIGAR\" onclick=\"window.location.href='/ledon'\">\r\n"
  end
   htmlstring = htmlstring.."</form>\r\n"
   htmlstring = htmlstring.."</body>\r\n"
   htmlstring = htmlstring.."</html>\r\n"
   sck:send(htmlstring)
end

function receiver(sck, data)
  if string.find(data, "GET /ledon")  then
   SendHTML(sck, true)
   gpio.write(LEDpin, gpio.HIGH)
   
  elseif string.find(data, "GET / ") or string.find(data, "GET /ledoff") then
   SendHTML(sck, false)
   gpio.write(LEDpin, gpio.LOW)   
   
  else
   sck:send("<h2>Not found...!!</h2>")
   sck:on("sent", function(conn) conn:close() end)
  end
end
