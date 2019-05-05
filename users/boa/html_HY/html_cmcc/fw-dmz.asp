<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!--系统默认模板-->
<HTML>
<HEAD>
<TITLE>DMZ Host</TITLE>
<META http-equiv=pragma content=no-cache>
<META http-equiv=cache-control content="no-cache, must-revalidate">
<META http-equiv=content-type content="text/html; charset=utf-8">
<META http-equiv=content-script-type content=text/javascript>
<!--系统公共css-->
<STYLE type=text/css>
@import url(/style/default.css);
</STYLE>
<!--系统公共脚本-->
<SCRIPT language="javascript" src="common.js"></SCRIPT>
<script type="text/javascript" src="share_cmcc.js"></script>
<SCRIPT language="javascript" type="text/javascript">
var links = new Array();
with(links){push(new it_nr("2_INTERNET_R_VID_2", new it("vpi", 0), new it("qos", 0), new it("vci", 0), new it("pcr", 0), new it("scr", 0), new it("mbs", 0), new it("encap", 0), new it("napt", 1), new it("cmode", 1), new it("brmode", 0), new it("AddrMode", 1), new it("pppUsername", ""), new it("pppAuth", 0), new it("pppServiceName", ""), new it("pppACName", ""), new it("pppCtype", 0), new it("ipDhcp", 1), new it("ipAddr", "0.0.0.0"), new it("remoteIpAddr", "0.0.0.0"), new it("netMask", "0.0.0.0"), new it("vlan", 1), new it("vid", 2), new it("vprio", 0), new it("vpass", 0), new it("itfGroup", 3), new it("PPPoEProxyEnable", 0), new it("PPPoEProxyMaxUser", 0), new it("applicationtype", 1), new it("ifIndex", 130817)));
}

function on_init()
{
	with ( document.forms[0] )
	{
		for(var i = 0; i < links.length; i++)
		{
			ifname.options.add(new Option(links[i].name, links[i].ifIndex));
		}
	}
}

function skip () { this.blur(); }
function saveClick()
{
  if (document.formDMZ.dmzcap.checked)
 	return true;

  if (!checkHostIP(document.formDMZ.ip, 1))
	return false;
	
	
  return true;
}

function updateState()
{
//  if (document.formDMZ.enabled.checked) {
  if (document.formDMZ.dmzcap.checked) {
 	enableTextField(document.formDMZ.ip);
  }
  else {
 	disableTextField(document.formDMZ.ip);
  }
}

function checkChange(cb)
{
	if(cb.checked==true){
		cb.value = 1;
	}
	else{
		cb.value = 0;
	}
	updateState();
}
</SCRIPT>
</head>

<body>
<blockquote>

<DIV align="left" style="padding-left:20px; padding-top:5px">
<form action=/boaform/formDMZ method=POST name="formDMZ">
<br><br>
来自网络侧的报文如果没有端口映射，会被转发到DMZ主机上。
<table border="0" cellpadding="0" cellspacing="0">
<tr>
	<td>接口名称: </td>
	<td>
		<select name="ifname" style="width:200px "> <option value=130817>2_INTERNET_R_VID_2</option>
 </select>
	</td>
</tr>
<tr><td>使能：</td>
      <td>
		<input type="checkbox" name="dmzcap" onChange="checkChange(this)" value='0'>
      </td>
</tr>
<tr>
	<td>LAN侧IP地址: </td>
	<td><input type="text" name="ip" size="15" maxlength="15" value=0.0.0.0 ></td>
</tr>
<tr>
	<td><input class="btnsaveup" type="submit" value="保存/应用" name="save" onClick="return saveClick()">&nbsp;&nbsp;</td>
</tr>
<tr><td>
   <br>
        <!--input type="reset" value="Reset" name="reset"-->
        <input type="hidden" value="/fw-dmz.asp" name="submit-url">
</td></tr>
</table>
<script> 
	ifIdx = 65535;
	if (ifIdx != 65535)
		document.formDMZ.ifname.value = ifIdx;
	else
		document.formDMZ.ifname.selectedIndex = 0;
	 updateState(); 
</script>
</form>
</DIV>
</blockquote>
</body>
</html>
