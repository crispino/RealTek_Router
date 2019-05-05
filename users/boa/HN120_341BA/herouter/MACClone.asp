<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>MAC地址克隆</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">

var clientListArray = new Array;

function checkMac(str){
	var len = str.length;
	if(len!=17)
		return false;

	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if (    (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
	return true;
}

function MACClonesubmit()
{
	if( document.getElementById("macCloneMac").value == "")
	{
		alert("MAC地址不能为空");
		return;
	}
	else if( document.getElementById("macCloneMac").value == "ff:ff:ff:ff:ff:ff")
	{
		alert("MAC地址不能为ff:ff:ff:ff:ff:ff");
		return;
	}
	else{
		if(!checkMac(document.getElementById("macCloneMac").value))
		{
			alert("MAC地址有误");
			return;
		}
		//document.MACClone.submit();		
		//alert("正在设置请稍等.....");
		Alert("正在保存设置，请稍候", 10);
		setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面', 20)", 10100);	

		
		  $.ajax({
      	type: "post",
        url: "/boafrm/MACCloneCfg",     
        data: $("#macCloneMac").serialize()  
        
      });

		
      
	}
}

$(function(){
	var currOpmode = "<%getHeInfo("opMode")%>";
    if(currOpmode==1){
		$('.ifdisabled').addClass('disabled');
		
		$('.disabled').click(function(event){  
	        return false;  
	    });
	}
	
})

</script>
</head>

<body>
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a class="ifdisabled" href="WANConfig.asp">WAN设置</a></li>
				<li><a href="DHCPbanding.asp">局域网设置</a></li>
				<li class="on"><a class="ifdisabled" href="MACClone.asp">MAC地址克隆</a></li>
				<li><a class="ifdisabled" href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <form method=post name="MACClone" action="/boafrm/MACCloneCfg">
        <div class="cfgright">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="25%">&nbsp;</td>
                <td width="75%">&nbsp;</td>
              </tr>
              <tr>
              	<input type="hidden" id="macCloneEnbl" name="macCloneEnbl" value="1">
              	<td><span>MAC 地址</span></td>      
					<td><div class="fix" >
							<div class="ganyin" onclick="hide('test1')" style=" width:250px;"></div>
							<div id="test1" style="display: none; " class="bm">
							<ul style=" width:250px;">	
								<% dhcpMacCloneClientList(); %>
							</ul>
							</div>
							</div>
									
<input class="selectinp" style="width: 230px;" type="text" id="macCloneMac" name="macCloneMac" value=" "/></td>
	  			
<!--              	
                <td><span>IP 地址</span></td>
                <td><div class="fix" style=" z-index:999;">
						<div class="ganyin" onClick="hide('test1')" style=" width:250px;"></div>
							<div id="test1" style="display: none; " class="bm">
							<ul style=" width:250px;">
								
								<td><span>MAC 地址</span></td>
<td><input class="selectinp" type="text" id="macCloneMac" name="macCloneMac" value=""/></td>

								<li  onClick="pick('测试1','select1','test1')" >测试1</li>
								<li  onClick="pick('测试2','select1','test1')" >测试2</li>
								<li  onClick="pick('测试3','select1','test1')" >测试3</li>
								<li  onClick="pick('测试4','select1','test1')" >测试4</li>
								<li  onClick="pick('测试5','select1','test1')" >测试5</li>

							</ul>
						</div>
					</div>
					<input type="hidden" id="macCloneEnbl" name="macCloneEnbl" value="1">
					<input class="selectinp" type="text" id="macCloneMac" name="macCloneMac" /></td> 
					        
-->         
						</tr>     
            </table>
            
            <div class="subpart">
                <ul>
                    <li><b onclick="MACClonesubmit();">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>

        </div>
        </form>
    </div>
</div>
<script type="text/javascript">
//	&macCloneEnbl=1&macCloneMac=0
function $$$$$(_sId){
 return document.getElementById(_sId);
 }
function hide(_sId){
	$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
 }
function pick(v,targetid,abc) {
	document.getElementById(targetid).value=v;
	hide(abc);
}


</script>
</body>
</html>
