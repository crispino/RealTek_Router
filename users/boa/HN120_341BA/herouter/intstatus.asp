<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<link href="css/and.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/jquery.vegas.min.js"></script>
<script type="text/javascript">
var http_request = false;
var rst_buf = new Array();
var quicksetupflag;
var pppoeretflag;

var mode = '<%getHeInfo("ConMode");%>';
var PingStatus = "<%getHeInfo("PingStatus");%>";

function isHiddenField(ishidden,hid_id)
{
	if(ishidden == 1){
		document.getElementById(hid_id).style.display = "none";
	}else{
		document.getElementById(hid_id).style.display = "block";
	}
} 
function parseResponse(rtBuf) {

/*response result:
			  cablePlug: pass
			  wanIp: pass
			  siteAccess: pass
			  PPPOEStatus:pass (pass:fail:timeout)
*/
	var wan_ok;
	$("#wait").css("display","none");
	rst_buf=rtBuf.split("\n");
	//WAN SIDE
	if( (rst_buf[0].indexOf("pass") == -1) || (rst_buf[1].indexOf("pass") == -1) ||(rst_buf[2].indexOf("pass") == -1)) {
		//alert("互联网不可达，请检查配置");
		if(rst_buf[0].indexOf("pass") == -1)
		{
		
		if(quicksetupflag=="1")
		    $("h2").text("网线未插,此设置将会在您插上网线后生效,继续设置请点击<跳过>按钮");
        else 
		    $("h2").text("网线未插,此设置将会在您插上网线后生效");
		}
		else if((mode == "PPPOE") && (rst_buf[3].indexOf("fail") != -1))
		{
		    $("h2").text("PPPOE账户或者密码错误，请检查后重新输入");
			pppoeretflag="1";
		}
		else if((mode == "PPPOE") && (rst_buf[3].indexOf("timeout") != -1))
		{
		if(quicksetupflag=="1")
		    $("h2").text("设置已生效但PPPOE拨号失败,可能是PPPOE账户/密码错误,未发现远程服务器,继续设置请点击<跳过>按钮");
		else 
		    $("h2").text("设置已生效但PPPOE拨号失败,未发现远程服务器,请检查宽带口的网线是否连接到外网");
		}
		else if((rst_buf[1].indexOf("pass") !=-1)&&(rst_buf[2].indexOf("pass") == -1) )
		{
		   
		if(quicksetupflag=="1")
		    $("h2").text("已获取到IP地址,但无法链接到外网,请在系统状态中查看DNS地址是否存在,继续设置请点击<跳过>按钮");
		else 
		    $("h2").text("已获取到IP地址,但无法链接到外网,请在系统状态中查看DNS地址是否存在");
		}
		else if((mode == "PPPOE") ){
		 
		if(quicksetupflag=="1")
		    $("h2").text("可能是PPPOE账户/密码错误，请<重新设置>，或PPPOE服务器暂无响应,请点击<跳过>按钮直接进行下一步操作");
		else 
		    $("h2").text("PPPOE服务器暂无响应,请稍后在系统状态中查看是否拨号成功");
		}
		else 
		{
		    $("h2").text("互联网不可达,请检查配置及网线连接");
		}

		$("div.c2").css("display", "none");
		$("div.c3").css("display", "none");
		$("ul").css("display","block");
	}else{
		$("h2").text("互联网畅通，可以上网冲浪");
		if(quicksetupflag=="1")
		{
			document.getElementById("lineinfo").innerHTML = "<p>3秒后自动跳到下一个配置页面 &gt;&gt;<span id=\"dd\">3</span></p>";
		}
		else
		{
			document.getElementById("lineinfo").innerHTML = "<p>3秒后自动关闭当前页面 &gt;&gt;<span id=\"de\">3</span></p>";
		}
		window.setInterval("run2();", 1000);
	}
	if(quicksetupflag=="1")
	{
		isHiddenField(0,"renetwizard");
		isHiddenField(1,"renetconfig");
		isHiddenField(0,"setquickwifi");
		isHiddenField(1,"closewebpage");
	}
	else
	{
		isHiddenField(1,"renetwizard");
		isHiddenField(0,"renetconfig");
		isHiddenField(1,"setquickwifi");
		isHiddenField(0,"closewebpage");
	}
}

function ReQueryNetStateResult()
{
	http_request.onreadystatechange = NetStateResult;
	http_request.open('POST', "/boafrm/netState", true);
	http_request.send("netState=true");
}

function NetStateResult() {
	if (http_request.readyState == 4) {
	//alert(http_request.status);
	if (http_request.status == 200) {
			parseResponse(http_request.responseText);
		}
		else
		{
			setTimeout(ReQueryNetStateResult,2000);
		}
	}
}

function requestNetState() {
		http_request = false;
		if (window.XMLHttpRequest) { // Mozilla, Safari,...
			http_request = new XMLHttpRequest();
			if (http_request.overrideMimeType) {
				http_request.overrideMimeType('text/xml');
			}
		} else if (window.ActiveXObject) { // IE
			try {
				http_request = new ActiveXObject("Msxml2.XMLHTTP");
			} catch (e) {
				try {
				http_request = new ActiveXObject("Microsoft.XMLHTTP");
				} catch (e) {}
			}
		}
		if (!http_request) {
			alert('诊断初始化失败');
			return false;
		}
		
		http_request.onreadystatechange = NetStateResult;
		http_request.open('POST', "/boafrm/netState", true);
		http_request.send("netState=true");
}

function reboot_internet() {
      $.ajax({
      	type: "post",
        url: "/boafrm/doInitInternet",     
        data: $("#internet_status").serialize(),    
        success: function(data){
        },
        error: function(data) {
        }
      })
}

function run(){
    var s = document.getElementById("wait");
    if(typeof s == 'undefined' || !s || s.innerHTML == 0){
        return false;
    }
    s.innerHTML = s.innerHTML * 1 - 1;
}
function run2(){
	if(quicksetupflag=="1")
	{
		var s = document.getElementById("dd");
		if(typeof s == 'undefined' || !s || s.innerHTML == 0){
			location.href="/herouter/QuickSetwifi.asp";
			return false;
		}
		s.innerHTML = s.innerHTML * 1 - 1;
	}
	else
	{
		var s = document.getElementById("de");
		if(s.innerHTML == 0){
			parent.TINY.box.hide();
			return false;
		}
		s.innerHTML = s.innerHTML * 1 - 1;
	}
}

function wizardreconfig(){
	window.location.href="QuickSetUp.asp?netmode="+pppoeretflag;
}

function renetconfig(){
	window.location.href="intconfig.asp";
}

function jump(){
	window.location.href="QuickSetwifi.asp";
}

$(document).ready(function(){

	quicksetupflag = GetQueryString("quicksetup");
	reboot_internet();
	document.getElementById("msg").innerHTML = "正在设置路由器，请稍候...<span id=\"wait\">60</span>秒";
	window.setInterval("run();", 1000);	
	setTimeout(requestNetState, 62000);

	if (mode == "DHCP"){
		document.getElementById("connectmode").innerHTML = "<b>上网方式：</b><span>自动获取（DHCP）</span>"; 
		$("li#pppoename").css("visibility", "hidden");
		$("li#pppoepwd").css("visibility", "hidden");
	}else if (mode == "STATIC"){
		$("li#pppoename").css("visibility", "hidden");
		$("li#pppoepwd").css("visibility", "hidden"); 
	}else if(mode == "PPPOE"){
		document.getElementById("connectmode").innerHTML = "<b>上网方式：</b><span>宽带拨号（PPPoE）</span>"; 
		$("li#pppoename").css("visibility", "visible");
		$("li#pppoepwd").css("visibility", "visible");
	}
	if(quicksetupflag=="1"){
  document.getElementById("Toptitle").innerHTML='<span class="Txttype">(第一步)互联网设置<em>--></em></span><span class="newHiType">(第二步)无线设置—>(第三步)完成</span>';
}

});

</script>

</head>

<body>
<form method="post" name="internet_status" id="internet_status" action="/boafrm/doInitInternet">
<div class="int">
	<h1 id="Toptitle" align="center">互联网设置</h1>
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
        <div class="cfgright">    	 	
            <div class="state">
					  	<div class="entstate">
				        <ul>
				          <li id="connectmode"><b>上网方式：</b><span>
				            <%getHeInfo("ConMode");%>
				          </span></li>				        	
				          <li id="pppoename"><b>PPPOE账户：</b><span>
				            <%getInfo("pppUserName");%>
				          </span></li>
				          <li id="pppoepwd"><b>PPPOE密码：</b><span>
				             <%getInfo("pppPassword");%>
				          </span></li>
				        </ul>
							</div>				                     	
						</div>            	
            <div class="subpart">
                <ul style="display:none">
					<li style="display:none;" id="renetwizard" onClick="wizardreconfig()"><b>重新设置</b></li>
					<li style="display:;" id="renetconfig" onclick="renetconfig();"><b>重新设置</b></li>
					<li style="display:none;" id="setquickwifi" onclick="jump();"><b>跳过</b></li>
					<li style="display:;" id="closewebpage" class="cancel" onclick="javascript:parent.TINY.box.hide();"><b>关闭</b></li>
                </ul>
            </div>
             <div id="lineinfo" style="line-height:30px; text-align:right;padding-top: 30px;" ></div>
        </div>
     
    </div>    
</div>
</form>
</body>
</html>
