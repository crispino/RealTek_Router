<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!--系统默认模板-->
<HTML>
<HEAD>
<TITLE>ALG On-Off</TITLE>
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
<SCRIPT language="javascript" type="text/javascript">

function checkChange(cb)
{
	if(cb.checked==true){
		cb.value = 1;
	}
	else{
		cb.value = 0;
	}
}

function apply_click()
{
	var l2tp_algonoff=<%getIndex("l2tp_algonoff");%>;
	var ipsec_algonoff=<%getIndex("ipsec_algonoff");%>;
	var h323_algonoff=<%getIndex("h323_algonoff");%>;
	var rtsp_algonoff=<%getIndex("rtsp_algonoff");%>;
	var sip_algonoff=<%getIndex("sip_algonoff");%>;
	var ftp_algonoff=<%getIndex("ftp_algonoff");%>;
	var pptp_algonoff=<%getIndex("pptp_algonoff");%>;
	var tftp_algonoff=<%getIndex("tftp_algonoff");%>;
	if(document.algof.l2tp_algonoff.value == 1)
	{
		if(l2tp_algonoff == 2)
		{
			alert('l2tp alg do not define!');
  			return false;

		}
	}
	if(document.algof.ipsec_algonoff.value == 1)
	{
		if(ipsec_algonoff == 2)
		{
			alert('ipsec alg do not define!');
  			return false;

		}
	}
	if(document.algof.h323_algonoff.value == 1)
	{
		if(h323_algonoff == 2)
		{
			alert('h323 alg do not define!');
  			return false;

		}

	}
	if(document.algof.rtsp_algonoff.value == 1)
	{
		if(rtsp_algonoff == 2)
		{
			alert('rtsp alg do not define!');
  			return false;

		}
	}

	if(document.algof.sip_algonoff.value == 1)
	{
		if(sip_algonoff == 2)
		{
			alert('sip alg do not define!');
  			return false;

		}
	}

	if(document.algof.ftp_algonoff.value == 1)
	{
		if(ftp_algonoff == 2)
		{
			alert('ftp alg do not define!');
  			return false;

		}

	}

	if(document.algof.pptp_algonoff.value == 1)
	{
		if(pptp_algonoff == 2)
		{
			alert('pptp alg do not define!');
  			return false;

		}

	}

	if(document.algof.tftp_algonoff.value == 1)
	{
		if(tftp_algonoff == 2)
		{
			alert('tftp alg do not define!');
  			return false;

		}

	}
	 
	 
	 
	 return true;
}
function LoadSetting()
{
	var l2tp_algonoff=<%getIndex("l2tp_algonoff");%>;
	var ipsec_algonoff=<%getIndex("ipsec_algonoff");%>;
	var h323_algonoff=<%getIndex("h323_algonoff");%>;
	var rtsp_algonoff=<%getIndex("rtsp_algonoff");%>;
	var sip_algonoff=<%getIndex("sip_algonoff");%>;
	var ftp_algonoff=<%getIndex("ftp_algonoff");%>;
	var pptp_algonoff=<%getIndex("pptp_algonoff");%>;
	var tftp_algonoff=<%getIndex("tftp_algonoff");%>;
	if(l2tp_algonoff==1)	
	{
		document.algof.l2tp_algonoff.checked = true;
			
	}
	else
	{
		document.algof.l2tp_algonoff.checked = false;
		
	}
	
	if(ipsec_algonoff==1)	
	{
		document.algof.ipsec_algonoff.checked = true;
	}
	else
	{
		document.algof.ipsec_algonoff.checked = false;
	}
	if(h323_algonoff==1)	
	{
		document.algof.h323_algonoff.checked = true;
			
	}
	else
	{
		document.algof.h323_algonoff.checked = false;
		
	}
	if(rtsp_algonoff==1)	
	{
		document.algof.rtsp_algonoff.checked = true;
	}
	else
	{
		document.algof.rtsp_algonoff.checked = false;
	}

	if(sip_algonoff==1)	
	{
		document.algof.sip_algonoff.checked = true;
			
	}
	else
	{
		document.algof.sip_algonoff.checked = false;
		
	}
	if(ftp_algonoff==1)	
	{
		document.algof.ftp_algonoff.checked = true;
	}
	else
	{
		document.algof.ftp_algonoff.checked = false;
	}

	if(pptp_algonoff==1)	
	{
		document.algof.pptp_algonoff.checked = true;
	}
	else
	{
		document.algof.pptp_algonoff.checked = false;
	}
	if(tftp_algonoff==1)	
	{
		document.algof.tftp_algonoff.checked = true;
	}
	else
	{
		document.algof.tftp_algonoff.checked = false;
	}

}

</SCRIPT>
</head>

<body onload="LoadSetting();" topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000">
<blockquote>
<DIV align="left" style="padding-left:20px; padding-top:5px">

<table  width=500>
<tr><td colspan=4></td></tr>
<form action=/boafrm/formALGOnOff method=POST name=algof>
<table>
<tr>
   <td ><input type="checkbox" name=l2tp_algonoff  onChange="checkChange(this)" checked>启用L2TP ALG</td><td ></td>
</tr>
<tr>
   <td ><input type="checkbox" name=ipsec_algonoff  onChange="checkChange(this)" checked>启用IPSec ALG</td><td ></td>
</tr>
<tr>
   <td ><input type="checkbox" name=h323_algonoff  onChange="checkChange(this)" checked>启用H.323 ALG</td><td ></td>
</tr>
<tr>
   <td ><input type="checkbox" name=rtsp_algonoff  onChange="checkChange(this)" checked>启用RSTP ALG</td><td ></td>
</tr>
<tr>
   <td ><input type="checkbox" name=sip_algonoff  onChange="checkChange(this)" checked>启用SIP ALG</td><td ></td>
</tr>
<tr>
   <td ><input type="checkbox" name=ftp_algonoff  onChange="checkChange(this)" checked>启用FTP ALG</td><td ></td>
</tr>
<tr>
   <td ><input type="checkbox" name=pptp_algonoff  onChange="checkChange(this)" checked>启用PPTP ALG</td><td ></td>
</tr>
<tr>
   <td ><input type="checkbox" name=tftp_algonoff  onChange="checkChange(this)" checked>启用TFTP ALG</td><td ></td>
</tr>
	
<tr>
	<td></td>
	<td></td>
</tr>
<tr>
  <td> <input type="hidden" value="/algonoff.asp" name="submit-url"></td>
  <td ><input  class="btnsaveup" type=submit value="保存/应用" name="apply" onClick="return apply_click()"></td>
  <td></td>
</tr>
</table>
</form>
</table>
</DIV>
</blockquote>
</body>
</html>
