<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>外网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
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
  }else{
  	document.quickSettingWan.conTp_radio[0].checked = true;    
      AutoChoose(0);
  }
  
	return true;
}

function cancel(){
	parent.TINY.box.hide();
}


</script>
</head>

<body onLoad="initValue()">
<form method=post name="quickSettingWan" id="quickSettingWan" action="/boafrm/setWan" >
<div class="int">
	<h1>外网设置<span>*请选择您的上网接入方式</span></h1>
    <div class="cfg">
        
            <table width="100%" border="0" cellspacing="0" cellpadding="0" style="padding-left: 100px;">
				<tr>
					<td width="30%">
						<input class="radiobox" type="radio" name="conTp_radio" onClick="switch_conType()" value="0">自动方式（DHCP)
					</td>
					<td width="70%">
						<input class="radiobox" type="radio" name="conTp_radio" onClick="switch_conType()" value="1" style="margin-left: 10px;">手动方式（静态IP）&nbsp;&nbsp;
						<input class="radiobox" type="radio" name="conTp_radio" onClick="switch_conType()" value="2" style="margin-left: 50px;">PPPOE方式
					</td>
				</tr>
				<tr id="ipAddr">
					<td><span>IP地址</span></td>
					<td><input class="inp1" type="text" name="staticIp" value="<% getInfo("wan-ip-rom"); %>"/></td>
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
					<td><input class="inp1" type="text" name="pppoeUser" value="<%getInfo("pppUserName");%>" /></td>
				</tr>
				<tr id="PPPOEPassword">
					<td><span>PPPOE密码</span></td>
					<td><input class="inp1" id="pppoePass" type="text" name="pppoePass" value="<% getInfo("pppPassword");%>" /></td>
				</tr>
				<tr id="MtuSetting">
					<td><span>MTU设置</span></td>
					<td><input class="inp1" id="MTU" type="text" name="MTU" value="<% getInfo("pppMtuSize");%>" />
						<i style="color: #FF0000;font-size: 10px;display: block;">默认是1492，请勿修改！</i>
					</td>
				</tr>
				<tr style="display: none; ">
					<td><input name="connectionType" id="connectionType" value="" style="display: none;" /></td>
					<td><input name="hostname" id="hostname" value="0" style="display: none;" /></td>
					<td><input name="macCloneEnbl" id="macCloneEnbl" value=0 style="display: none;" /></td>
					<td><input name="macCloneMac" id="macCloneMac" value="" style="display: none;" /></td>
					<td><input name="pppoeOPMode" id="pppoeOPMode" value="KeepAlive" style="display: none;" /></td>
					<td><input name="pppoeRedialPeriod" id="pppoeRedialPeriod" value="60" style="display: none;" /></td>
				</tr>
			</table>
            <div class="subpart">
                <ul>
                    <li><b onclick="javascript:wanSetting_apply();">保存</b></li>
                    <li><b class="cancel" onclick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
           </div>
    </div>
</div>
</form>
</body>
</html>
