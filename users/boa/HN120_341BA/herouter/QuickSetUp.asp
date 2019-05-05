<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>设置向导一</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">
	
var mode = '<%getHeInfo("checkMode")%>';
var isWanPortPlug = "<%getHeInfo("WanPortPlug");%>";
var PingStatus = "<%getHeInfo("PingStatus");%>";
var nInitSelectType = <%getIndex("wanDhcp");%>;
var onWizard = <%getHeInfo("isNeedWizard");%>;
var pppoereqflag;
var isIEVers7to9;

function AutoChoose(isAuto){

    var visitBuf;
    var conType;
    var pppvisitBuf;
    
    if(isAuto == 0){
    	  visitBuf = "none";
    	  pppvisitBuf = "none";
    	  conType = "DHCP";

    }else if(isAuto == 1){
    	  visitBuf = "";
    	  pppvisitBuf = "none";
    	  conType = "STATIC";
    }else if(isAuto == 2){
    		visitBuf = "none";
    		pppvisitBuf = "table-row";
    		conType = "PPPOE";
    }
    
    /*
    document.getElementById("ipAddr").style.display = visitBuf;
    document.getElementById("netMask").style.display = visitBuf;
    document.getElementById("gateway").style.display = visitBuf;
    document.getElementById("priDNS").style.display = visitBuf;
    document.getElementById("secDNS").style.display = visitBuf;
    */
		document.getElementById("PPPOEUser").style.display = pppvisitBuf;
		document.getElementById("PPPOEPassword").style.display = pppvisitBuf;
    document.quickSettingWan.connectionType.value = conType;  
    
} 

function switch_conType(){
	
	if (document.quickSettingWan.conTp_radio[0].checked == true){
		AutoChoose(0);
	}
	else if(document.quickSettingWan.conTp_radio[1].checked == true){
		AutoChoose(1);
	}
	else if(document.quickSettingWan.conTp_radio[2].checked == true){
		AutoChoose(2);
	}
}

function check_value(){
	var newSelect = 0;

	if (document.quickSettingWan.conTp_radio[0].checked == true) { //DHCP
		newSelect = 1;
	}
	else if (document.quickSettingWan.conTp_radio[1].checked == true) {      //STATIC
		newSelect = 0;
		if (!checkIpAddr(document.quickSettingWan.staticIp, false))
			return false;
		if (!checkIpAddr(document.quickSettingWan.staticNetmask, true))
			return false;
		if (document.quickSettingWan.staticGateway.value != "")
			if (!checkIpAddr(document.quickSettingWan.staticGateway, false))
				return false;
		if (document.quickSettingWan.staticPriDns.value != "")
			if (!checkIpAddr(document.quickSettingWan.staticPriDns, false))
				return false;
		if (document.quickSettingWan.staticSecDns.value != "")
			if (!checkIpAddr(document.quickSettingWan.staticSecDns, false))
				return false;
	} 
	else if (document.quickSettingWan.conTp_radio[2].checked == true) { // PPPOE
		newSelect = 3;
		if (document.quickSettingWan.pppoeUser.value == "" ||
				document.quickSettingWan.pppoePass.value == "")
		{
			alert("PPPoE账户或密码不能为空！");
			return false;
		}
	}	
	else
		return false;

	if (newSelect != nInitSelectType)
	{
		return true;
	}

	return true;
}

function afterSubmit(){
  alert("设置成功");	
  parent.TINY.box.hide();
  parent.location.reload();
}

function wanSetting_apply(){
	var newSelect = 0;

	if (document.quickSettingWan.conTp_radio[0].checked == true) { //DHCP
		newSelect = 1;
	}
	else if (document.quickSettingWan.conTp_radio[1].checked == true) {      //STATIC
		newSelect = 0;
	} 
	else if (document.quickSettingWan.conTp_radio[2].checked == true) { // PPPOE
		newSelect = 3;
				
		if (check_value() == false)
		{
			return;
		}
	}	

	// if the internet is ok, and there is no any changed, you can jump to wifi asp
	if ((PingStatus == "pass") && (newSelect == nInitSelectType))
	{
		location.href="/herouter/QuickSetwifi.asp";
		return;
	}
	
	if (check_value() == true || PingStatus != "pass")
	{
		document.quickSettingWan.submit();
  //    setTimeout('afterSubmit()',10000);
	}
}


function initNoPassText()
{
	if(mode == "DHCP")
	{
		//document.getElementById("fail").innerHTML="<b class=\"err\">经检测，您的上网方式可能为PPPOE，请手动设置账号和密码</b>";
		$("#msg").text("经检测：您的上网方式为自动获取！");
	}else if(mode == "STATIC"){ //usual look as DHCP(AUTO) mode
		//document.getElementById("fail").innerHTML="<b class=\"err\">经检测，您的上网方式可能为PPPOE，请手动设置账号和密码</b>";
		$("#msg").text("经检测：您的上网方式为自动获取！");
	}else if(mode == "PPPOE"){ //usual look as DHCP(AUTO) mode
		//document.getElementById("fail").innerHTML="<b class=\"err\">经检测，您的上网方式可能为DHCP，请手动设置</b>";
		$("#msg").text("经检测：您的上网方式可能为宽带拨号!");
	}
  
	return true;
}

function run(){
	var s = document.getElementById("wait");
	if(typeof s == 'undefined' || !s || s.innerHTML == 0){
		return false;
	}
	s.innerHTML = s.innerHTML * 1 - 1;
}

function cancel(){
	
	document.getElementById("msg").innerHTML = "正在重新检测上网方式，请等待...<span id=\"wait\">3</span>秒";
	$("b.cancel").text("检测中...");
	window.setInterval("run();", 1000);
	setTimeout("location.reload();", 3000);
	
}
function pppoeSetting_showpasswd(){
	
	if (document.getElementById("pppoePass").type=="password" )
	{
document.getElementById("pppoePass").value="";

		document.getElementById("pppoePass").type="text";
	}
}
	


function getBrowserInfo()
{
	var agent = navigator.userAgent.toLowerCase() ;
	
	var IEVersion;
	var regStr_ie = /msie [\d.]+;/gi ;

	//IE
	if(agent.indexOf("msie ") > 0)
	{
		IEVersion = agent.match(regStr_ie);
		if (agent.match(/7./i)=="7.")
			isIEVers7to9 = 1;
		else if (agent.match(/8./i)=="8.")
			isIEVers7to9 = 1;
		else if (agent.match(/9./i)=="9.")
			isIEVers7to9 = 1;
		else
			isIEVers7to9 = 0;
	}
	else
		isIEVers7to9 = 0;
}


$(document).ready(function(){
	var pppvisitBuf;
	//PingStatus = "pass";
	//isWanPortPlug = "pass";
	//alert(isWanPortPlug);
	pppoereqflag = GetQueryString("netmode");
	
	getBrowserInfo();

	if(<% getIndex("opMode"); %>!=0){
		location.href="/herouter/QuickSetwifi.asp";
		return;
	}
	
	if (PingStatus != "pass")	{ //未连接到互联网
	
		if (isWanPortPlug != "pass")
		{
			$("#msg").text("经检测：宽带口网线未插入！");
			$("table:gt(0)").css({"display":"none"});
			$("b#next").css({"display":"none"});
			if(isIEVers7to9 == 1)
			{
				$("b.cancel").css({"bottom":"0px"});
			}
		}
		else
		{
			$("table:gt(0)").css({"display":"block"});
			$("b#next").css({"display":"block"});
			$("b.cancel").css({"display":"none"});
			initNoPassText();
		}
		/*if (mode != "DHCP"){ 
			document.quickSettingWan.conTp_radio[2].checked = true;
			nInitSelectType = 0;
			document.quickSettingWan.connectionType.value="PPPOE";
		}
		else{
			if (pppoereqflag != "1")
			{  // pppoe, but failed for connect resaon
				document.quickSettingWan.conTp_radio[0].checked = true;
				nInitSelectType = 2;
				pppvisitBuf = "hidden";
				document.getElementById("PPPOEUser").style.visibility = pppvisitBuf;
				document.getElementById("PPPOEPassword").style.visibility = pppvisitBuf;
				document.quickSettingWan.connectionType.value="DHCP";
			}
			else
			{  // pppoe, but failed for author, only used for 网络状态通过点击重新设置
				document.quickSettingWan.conTp_radio[2].checked = true;
				nInitSelectType = 0;
				document.quickSettingWan.connectionType.value="PPPOE";
			}
		}*/
	}else{ //已经连接到互联网
		$("b.cancel").css({"display":"none"});
	  
		if (mode == "PPPOE"){ 
			//document.quickSettingWan.conTp_radio[2].checked = true;  
			//AutoChoose(2);
			$("#msg").text("经检测：您的上网方式可能为宽带拨号!");
		}
		else{
			//document.quickSettingWan.conTp_radio[0].checked = true;    
			//AutoChoose(0);
			$("#msg").text("经检测：您的上网方式为自动获取！");
		}
	}

	if(onWizard!=1){
		if(nInitSelectType==3){
			document.quickSettingWan.conTp_radio[2].checked = true;    
			AutoChoose(2);
		}
		else{
			document.quickSettingWan.conTp_radio[0].checked = true;  
			AutoChoose(0);
		}
	}
	else{
		if (mode == "PPPOE"){ 
			document.quickSettingWan.conTp_radio[2].checked = true;  
			AutoChoose(2);
		}
		else{
			document.quickSettingWan.conTp_radio[0].checked = true;    
			AutoChoose(0);
		}
	}
});
</script>
</head>
	
<body>
<form method=post name="quickSettingWan" id="quickSettingWan" action="/boafrm/QuickSetUp" >
<div class="int">
<h1 align="center" ><span class="Txttype">(第一步)互联网设置<em>--></em></span><span id="step3">(第二步)无线设置—>(第三步)完成</span></h1>
			<br>
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
    <div class="cfg">
			<!--div id="fail" class="c1" style="width:500px"><b class="err">经检测，您的上网方式可能为DHCP，请手动设置账号和密码</b></div>
			<div id="pass" class="c1" style="width:360px"><b>经检测，您的上网方式为DHCP</b></div-->
     	<div style="padding-left: 160px;padding-top: 10px;height: 170px;" >
			<table width="70%" border="0" cellspacing="0" cellpadding="0">
				<tr>
					<td width="40%">您可以选择上网方式：</td>
					<td width="5%">&nbsp;</td>
					<td width="55%">
						<input class="radiobox" type="radio" name="conTp_radio" onClick="switch_conType()" value="0">自动获取（DHCP）<br>
						<br>
						<div style="display:none">
							<input class="radiobox" type="radio" name="conTp_radio" onClick="switch_conType()" value="1">静态方式（静态IP）<br>
						</div>
						<input class="radiobox" type="radio" name="conTp_radio" onClick="switch_conType()" value="2">宽带账号（PPPoE）
					</td><br><br>
				</tr>
		
			</table>
			<table style="height: 72px;" width="70%" border="0" cellspacing="0" cellpadding="0">
				<br>
				<tr id="PPPOEUser">
					<td>PPPoE账户&nbsp;</td>
					<td><input class="inp2" type="text" name="pppoeUser" value="<%getInfo("pppUserName");%>" /></td>
				</tr>
				<tr id="PPPOEPassword">
					<td>PPPoE密码&nbsp;</td>
					<td><input class="inp2" id="pppoePass" onclick="pppoeSetting_showpasswd()" type="password" name="pppoePass" value="<%getInfo("pppPassword");%>" /></td>
				</tr>
			</table>
			<div style="display: none; ">
				<input name="connectionType" id="connectionType" value="" style="display: none;" />
				<input name="hostname" id="hostname" value="0" style="display: none;" />
				<input name="macCloneEnbl" id="macCloneEnbl" value=0 style="display: none;" />
				<input name="macCloneMac" id="macCloneMac" value="" style="display: none;" />
				<input name="pppoeOPMode" id="pppoeOPMode" value="" style="display: none;" />
				<input name="pppoeRedialPeriod" id="pppoeRedialPeriod" value="" style="display: none;" />
			</div>
			
		</div>
		<div class="subpart">
			<ul>
				<li><b id="next" onclick="javascript:wanSetting_apply();">下一步</b></li>
				<li><b id="cancel" class="cancel" onclick="cancel();">再次检测</b></li>
			</ul>
		</div>
    </div>
   
</div>
</form>
</body>
</html>
