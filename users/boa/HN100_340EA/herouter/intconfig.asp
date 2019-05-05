<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:410px; width:670px; overflow:hidden; background:#fff url(images/fullbg.gif) repeat-x 0 -2px; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; }
.int h1 span{ font-size:12px; color:#919191; padding-left:15px; font-weight:normal;}
.mainsetting{ padding:30px; }
.mainsetting h2{ font-size:14px; line-height:38px;}
.c1{ height:48px;}
.c1 b{ display:block; height:48px; cursor:pointer; width:190px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.c1 a{ background:url(images/help.png) no-repeat center center; display:block; float:left; height:48px; width:30px;}
.c1 b:hover{ background:#56bbcb;}
.c1 span{  font-size:12px; color:#919191; padding:0 0 0 15px; float:left; font-weight:normal; line-height:48px;}
.c1 .err{ background:#e71b1b;}
.c1 .err:hover{ background:#da1515;}
.mainsetting h3{ font-size:24px; font-weight:normal; line-height:50px; color:#29b8e4;}
.c2{ border-top:1px #e6e6e6 solid; margin-top:80px; line-height:30px;}

.cfg{ padding:15px 0; height:330px;}
.cfgleft{ float:left; width:185px; height:330px; background:url(images/cfgleftbg.png) repeat-y;}
.cfgleft ul{}
.cfgleft li{ cursor:pointer; height:40px; line-height:40px; font-size:14px; color:#666; border-left:4px #dfdfdf solid ; padding-left:12px;}
.cfgleft li:hover{ border-left:4px #1e9ec5 solid;  font-weight:bold;}
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:16px; background:#fff url(images/arrow1.png) no-repeat 170px center;}
.cfgright{padding-left:100px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:100px; font-size:14px;}
.cfgright .spheight{padding-left:100px; height:60px; }
.cfgright td label{ font-size:14px; line-height:60px;}
.radiobox{ margin-top:-2px;}
.inp1 {height:26px; border:1px #d2d2d2 solid; width:160px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.subpart{ height:48px; padding-left:120px; padding-top:25px;}
.subpart ul{}
.subpart li{ float:left; padding-right:10px;}
.subpart b{ display:block; height:48px; cursor:pointer; width:170px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.subpart b:hover{ background:#56bbcb;}
.subpart .cancel{ width:110px; background:#99e4f0; }
.STYLE1 {
	color: #FF0000;
	font-size: 10px;
}
-->
</style>

<script type="text/javascript">
	
var mode = "<% getHeInfo("ConMode"); %>";
 
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
    		pppvisitBuf = "";
    		conType = "PPPOE";
    }
    
    document.getElementById("ipAddr").style.display = visitBuf;
    document.getElementById("netMask").style.display = visitBuf;
    document.getElementById("gateway").style.display = visitBuf;
    document.getElementById("priDNS").style.display = visitBuf;
    document.getElementById("secDNS").style.display = visitBuf;
		document.getElementById("PPPOEUser").style.display = pppvisitBuf;
		document.getElementById("PPPOEPassword").style.display = pppvisitBuf;
document.getElementById("MtuSetting").style.display = pppvisitBuf;
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
	if (document.quickSettingWan.conTp_radio[0].checked == true) { //DHCP
	}
	else if (document.quickSettingWan.conTp_radio[1].checked == true) {      //STATIC
		if (!checkIpAddr(document.quickSettingWan.staticIp, false))
			return false;
		if (!checkIpAddr(document.quickSettingWan.staticNetmask, true,'子网掩码'))
			return false;
//		if (document.quickSettingWan.staticGateway.value != "")
			if (!checkIpAddr(document.quickSettingWan.staticGateway, false,'网关'))
				return false;
//		if (document.quickSettingWan.staticPriDns.value != "")
			if (!checkIpAddr(document.quickSettingWan.staticPriDns, false,'首选DNS'))
				return false;
//		if (document.quickSettingWan.staticSecDns.value != "")
			if (!checkIpAddr1(document.quickSettingWan.staticSecDns, false))
				return false;
	} 
	else if (document.quickSettingWan.conTp_radio[2].checked == true) { //PPPOE
				if(document.quickSettingWan.pppoeUser.value == ""){
					alert("请输入账户名称！");
					return false;
				}
				if(document.quickSettingWan.pppoePass.value == ""){
					alert("请输入账户密码！");
					return false;
				}
				if(document.quickSettingWan.MTU.value == ""){
					alert("请输入MTU！");
					return false;
				}
		
//                         if (document.quickSettingWan.MTU.value != "")
				if (!checkMtu(document.quickSettingWan.MTU.value))
				return false;
	}	
	else
		return false;

	return true;
}

function afterSubmit(){
  alert("设置成功");	
  parent.TINY.box.hide();
  parent.location.reload();
}

function wanSetting_apply(){
	
	if (check_value() == true)
	{
		document.quickSettingWan.submit(); 
		//parent.showConfProcess();
	}
}


function initValue(){
  
  if(mode == "DHCP"){
      document.quickSettingWan.conTp_radio[0].checked = true;    
      AutoChoose(0);
       
  }else if(mode == "STATIC"){ //usual look as DHCP(AUTO) mode
  	  document.quickSettingWan.conTp_radio[1].checked = true;  
  	  AutoChoose(1);
  }else if(mode == "PPPOE"){ //usual look as DHCP(AUTO) mode
  	  document.quickSettingWan.conTp_radio[2].checked = true;  
  	  AutoChoose(2);
  }
  
	return true;
}

function cancel(){
	parent.TINY.box.hide();
}
function pppoeSetting_showpasswd(){
	
	if (document.getElementById("pppoePass").type=="password" )
	{
document.getElementById("pppoePass").value="";

		document.getElementById("pppoePass").type="text";
	}
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="quickSettingWan" id="quickSettingWan" action="/boafrm/setWan" >
<div class="int">
	<h1>外网设置<span>*请选择您的上网接入方式</span></h1>
    <div class="cfg">
        <div class="cfgright">
            <table width="100%" border="0" cellspacing="0" cellpadding="0" >
              <tr>
                <td width="35%" >
                <input class="radiobox" type="radio" name="conTp_radio"  onClick="switch_conType()"  value="0">自动方式（DHCP)
                </td>
                <td width="35%" >
                        				
								<input class="radiobox" type="radio" name="conTp_radio"  onClick="switch_conType()"  value="1">手动方式（静态IP）
								
                </td>
                <td width="30%" >
                	<input class="radiobox" type="radio" name="conTp_radio"  onClick="switch_conType()"  value="2">PPPOE方式		
                	</td>
              </tr>
              <tr id="ipAddr">
                <td><span>IP地址</span></td>
                <td><input class="inp1" type="text" name="staticIp" value="<% getInfo("wan-ip-rom"); %>"/></td>
              	<td width="30%" ></td>
              </tr>
              <tr id="netMask">
                <td><span>子网掩码</span></td>
                <td><input class="inp1" type="text" name="staticNetmask" value="<% getInfo("wan-mask-rom"); %>" /></td>
              </tr>
              <tr id="gateway">
                <td><span>网关</span></td>
                <td><input class="inp1" type="text" name="staticGateway" value="<% getInfo("wan-gateway-rom"); %>" /></td>
              </tr>
              <tr id="priDNS">
                <td><span>首选DNS</span></td>
                <td><input class="inp1" type="text" name="staticPriDns" value="<% getInfo("wan-dns1"); %>" /></td>
              </tr>
              <tr id="secDNS">
                <td><span>备用DNS</span></td>
                <td><input class="inp1" type="text" name="staticSecDns" value="<% getInfo("wan-dns2"); %>" /></td>
              </tr>
              <tr id="PPPOEUser">
              	<td><span>PPPOE账户</span></td>	
  							<td><input class="inp1" type="text"  name="pppoeUser" value="<%getInfo("pppUserName");%>" /></td>            	
              </tr>
							<tr id="PPPOEPassword">
								<td><span>PPPOE密码</span></td>	
								<td><input class="inp1" id="pppoePass" onclick="pppoeSetting_showpasswd()" type="password"  name="pppoePass" value="<% getInfo("pppPassword");%>"  /></td>            	
							</tr>
<tr id="MtuSetting">
							<td><span>MTU设置</span></td>	
								<td><input class="inp1" id="MTU"  type="text"  name="MTU" value="<% getInfo("pppMtuSize");%>"  /><li class="STYLE1">默认是1492，请勿修改！</li></td>
								
					</tr>		
            </table>
            <tr style="display: none; ">
							  <td><input name="connectionType" id="connectionType" value=""  style="display: none;"/></td>
							  <td><input name="hostname" id="hostname" value="0"  style="display: none;"/></td>
							  <td><input name="macCloneEnbl" id="macCloneEnbl" value=0  style="display: none;"/></td>
							  <td><input name="macCloneMac" id="macCloneMac" value=""  style="display: none;"/></td>
							  <td><input name="pppoeOPMode" id="pppoeOPMode" value="KeepAlive"  style="display: none;"/></td>
							  <td><input name="pppoeRedialPeriod" id="pppoeRedialPeriod" value="60"  style="display: none;"/></td>
						 </tr>
            <div class="subpart">
                <ul>
                    <li><b onclick="javascript:wanSetting_apply();">保存</b></li>
                    <li><b class="cancel" onclick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>

        </div>
    </div>
</div>
</form>
</body>
</html>
