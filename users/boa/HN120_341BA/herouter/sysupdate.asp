<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>系统升级</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
</head>

<script language="JavaScript" type="text/javascript">
//document.write('<div id="loading" style="display: none;"><br><br><br><br><br>上传固件 <br><br> 请耐心等待......</div>');
	
var isneedUpgrade = '<%getHeInfo("isneedUpgrade");%>';
var uploadingFirmware='0';

var _singleton = 0;	
function uploadFirmwareCheck()
{
	if(_singleton)
		return false;
		
	if(document.UploadFirmware.filename.value == "")
	{
		document.getElementById("note").style.display="none";
		document.getElementById("error").style.display="block";
		return false;
	}
	else
	{
		document.getElementById("error").style.display="none";	
		document.getElementById("note").style.display="block";
	}

	//document.getElementById("loading").style.display="block";	
	_singleton = 1;

//	document.getElementById("imagefile").style.display="none";
	document.getElementById("note").style.display="none";
	document.getElementById("info").style.display="block";
	
	document.getElementById("UploadFirmware").submit();
	
		
	return true;
}


function uploadFirmwareRemote()
{
	if(isneedUpgrade !="1")
	{
		//alert("当前版本已是最新版本，无需在线升级");
		return;
	}

	if(_singleton)
		return false;
		
	//document.getElementById("loading").style.display="block";	
	_singleton = 1;
	
	document.getElementById("RemoteUpgrade").submit();
	//parent.showupdating();
		
	return true;
}


function init()
{
	
	uploadingFirmware=GetQueryString("uploadingFirmware");
  	if(uploadingFirmware=="1"){
		document.getElementById("info").innerHTML = "文件上传完成!";
//		document.getElementById("imagefile").style.display="none";
		document.getElementById("note").style.display="none";
		document.getElementById("info").style.display="block";
		parent.showLocalupdating();
		return;
	}
	else if(uploadingFirmware=="2"){
		parent.showupdating();
		return;
	}
}


//window.onload = function(){
//	var uploadFW = document.getElementById('uploadFWLocation');
//	var fileName = document.getElementById('file_name');
//	uploadFW.onchange = function(){
//		if(uploadFW.files[0]){
//			fileName.innerHTML = '系统升级文件：' + uploadFW.files[0].name;
//		}
//		
//	}
//}
function changeFn(that){
	var fileName = document.getElementById('file_name');
	if(that.files[0]){
			fileName.innerHTML = '系统升级文件：' + that.files[0].name;
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

<body  onload="init()">
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a class="ifdisabled" href="WANConfig.asp">WAN设置</a></li>
				<li><a href="DHCPbanding.asp">局域网设置</a></li>
				<li><a class="ifdisabled" href="MACClone.asp">MAC地址克隆</a></li>
				<li><a class="ifdisabled" href="systemtime.asp">系统时间管理</a></li>
				<li class="on"><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        
        <div class="cfgright">  			
			<div class="update">
				<div>
					当前版本：<%getInfo("fwVersion");%>
				</div>
				<br><br><br>
			</div>
				<div class="style2">
					<p id="note" style="display: block;">注意：升级过程中请保持电源供电！否则可能损坏路由器！</p>
					<p id="error" style="display: none;">固件升级 <br>请指定文件.</p>
					<p id="info" style="display: none;">文件上传中，请等待...</p>
					<div class="subbtn2">
						<form method="post" id="UploadFirmware" name="UploadFirmware" action="/boafrm/heUIUpload" enctype="multipart/form-data">
							<span onClick="uploadFirmwareCheck();">本地升级</span>
							<span class="upload"><div class="uploadbtn"><input id="uploadFWLocation" name="filename" onchange="changeFn(this)" type="file" /></div>选择文件</span>
						</form>
				</div>
			</div>
			<b id="file_name" style="margin-left: 16px;line-height: 70px; font-size: 16px;font-weight: normal;"></b>
        </div>
    </div>
</div>
</body>
</html>
