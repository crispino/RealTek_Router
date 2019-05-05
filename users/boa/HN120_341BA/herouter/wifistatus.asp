<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>设置向导三</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<link href="css/and.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">

var WIFIAuthMode= '<%getHeInfo("WIFIAuthMode", "wlan2");%>';
var WIFIAuthMode_5G = '<%getHeInfo("WIFIAuthMode", "wlan1");%>';

function reboot_wifi() {
      $.ajax({
      	type: "post",
        url: "/boafrm/doInitInternet",     
        data: $("#wifi_status").serialize(),    
        success: function(data){
        },
        error: function(data) {
        }
      })
}

var timer;

function run(){
    var s = document.getElementById("wait");
     s.innerHTML = s.innerHTML * 1 - 1;
    if(s.innerHTML == 1){
    	clearInterval(timer);
        return false;
    }
   
}

function setTitle()
{
	$("#step1").text("(第一步)无线设置—>");
	$("#step2").text("(第二步)完成");
}

$(document).ready(function(){
	
	if(<% getIndex("opMode"); %>!=0){
		setTitle();
	}
	
	var WIFIAuthModeArray = WIFIAuthMode.split(";");
	if(WIFIAuthModeArray[0] == "OPEN"){
		$("#netpwd").text("无");
	}
	setTimeout("reboot_wifi();", 0);
	//setTimeout('$("h2").text("正在设置路由器，请稍侯...");', 0);
	document.getElementById("msg").innerHTML = "正在设置路由器，请稍候...<span id=\"wait\">10</span>秒";
	timer = window.setInterval("run();", 1000);
	setTimeout('$("#wait").css("display","none"); $("h2").text("设置成功，无线网络将断开，请在1分钟后重新进入");', 10000);
	setTimeout("parent.reload_main();", 30000); 
});	

</script>
</head>
<body>
<form method="post" name="wifi_status" id="wifi_status" action="/boafrm/doInitInternet">	
<div class="int">
<h1 align="center">
	<span id="step1">(第一步)互联网设置—>(第二步)无线设置—></span>
	<span id="step2" class="Txttype">(第三步)完成</span>
</h1>
    <div class="cfg">
				<div class="box">   
					<div class="top">
						<div class="r1"></div>
						<div class="r2"></div>
						<div class="r3"></div>
						<div class="r4"></div>
					</div>
					<div class="content">
							<table style="margin:auto;">
	        			<tr>
									<td><img src="./images/check3_07.png"/></td>
									<td>&nbsp;&nbsp;</td>
	     						<td>
	     							<h2 id="msg"></h2>
	     						</td>
	        			</tr> 
							</table>					
					</div>
					<div class="bottom">
						<div class="r4"></div>
						<div class="r3"></div>
						<div class="r2"></div>
						<div class="r1"></div>
					</div>
				</div>        		     	
        				       		
			  	<div class="entstate">
		        <!--h3>*无线设置完成，请手动连接到新的Wi-Fi</h3-->
		        <ul>
		          <li><b>网络名称：</b><span>
		          		<% getHeInfo("wizard_SSID", "wlan2"); %>				            	
		          </span></li>
		          <li><b>网络密码：</b><span id="netpwd" style="word-warp:break-word;word-break:break-all">
		            	<% getHeInfo("pskValue", "wlan2"); %>	
		          </span></li>
		          <li><b>网络名称：</b><span>
		          		<% getHeInfo("wizard_SSID", "wlan1"); %>				            	
		          </span></li>
		          <li><b>网络密码：</b><span id="netpwd" style="word-warp:break-word;word-break:break-all">
		            	<% getHeInfo("pskValue", "wlan1"); %>	
		          </span></li>
		        </ul>
		      </div>            	
			
       
        <div style="line-height:30px; text-align:right;padding-top: 30px;" ><p>设置完成后，向导自动关闭 &gt;&gt;</p></div>
    </div>    
</div>
</form>
</body>
</html>
