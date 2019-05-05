<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/style_main.css" rel="stylesheet" type="text/css" />
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
<!--
.update{ padding:30px 10px 0 0; border-bottom:1px #d3d3d3 solid;}
.style2{ padding-top:10px; height:90px;}
.style2 p{float:left; color:#FF0000; display:block; width:170px; line-height:20px; padding:4px 0 4px 35px; background:url(images/check3_07.png) no-repeat 2px center;}
.style2 .subbtn2{ float:right; height:48px; width:240px;}
.style2 .subbtn2 span{ float:right; cursor:pointer; display:block; height:48px; width:110px; margin-left:10px;line-height:48px; font-size:16px;background:#77d1df; color:#fff; text-align:center;}
.style2 .subbtn2 span:hover{ background:#56bbcb;}
.style2 .subbtn2 em{ display:block; width:240px; line-height:28px; font-style:normal; color:#ff6c00; text-align:right; }
.style2 .subbtn2 .upload{  background:#ff6c00;}
.style2 .subbtn2 .upload:hover{ background:#ff8227;}
.uploadbtn{ position:absolute; display:block; height:48px; width:110px; border:1px #333 solid;filter:alpha(opacity=0);-moz-opacity:0;-khtml-opacity: 0;opacity: 0; cursor:pointer;}
.uploadbtn input{height:48px; width:110px; cursor:pointer;}
-->
</style>
</head>

<!--link rel="stylesheet" href="/style/normal_ws.css" type="text/css"-->
<!--meta http-equiv="content-type" content="text/html; charset=utf-8"-->
<script type="text/javascript" src="/herouter/lang/b28n.js"></script>
<style type="text/css">
<!--
#loading {
       width: 250px;
       height: 200px;
       background-color: #3399ff;
       position: absolute;
       left: 50%;
       top: 50%;
       margin-top: -150px;
       margin-left: -250px;
       text-align: center;
}
-->
</style>


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

	document.getElementById("imagefile").style.display="none";
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
		document.getElementById("imagefile").style.display="none";
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
</script>

<body  onload="init()">
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a href="WANConfig.asp">WAN设置</a></li>
				<li><a href="DHCPbanding.asp">局域网设置</a></li>
				<li><a href="MACClone.asp">MAC地址克隆</a></li>
				<li><a href="systemtime.asp">系统时间管理</a></li>
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
					<!--p id="loading" style="display: none;">Uploading firmware <br>Please be patient and don't remove usb device if it presented...</p-->
					<div class="subbtn2">
						<form method="post" id="UploadFirmware" name="UploadFirmware" action="/boafrm/heUIUpload" enctype="multipart/form-data">
							<span onClick="uploadFirmwareCheck();">本地升级</span>
							<span class="upload"><div class="uploadbtn"><input id="imagefile" name="filename" type="file" /></div>选择文件</span>
						</form>
				</div>
			</div>
        </div>
    </div>
</div>
</body>
</html>
