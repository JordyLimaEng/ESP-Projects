<script language="javascript">



function id( el ){
  return document.getElementById( el );
}
	var xmlhttp = getXmlHttpRequest();
	
	function getXmlHttpRequest() {
		if (window.XMLHttpRequest) {
			return new XMLHttpRequest();
		} else if (window.ActiveXObject) {
			return new ActiveXObject("Microsoft.XMLHTTP");
		}
	}

    function status() {
        var IP = "http://192.168.0.20/";

url = IP + "status";
xmlhttp.open("POST", url, true);
xmlhttp.onreadystatechange = function(){

if (xmlhttp.readyState==4){
        //document.getElementById("Resposta_da_ESP").innerHTML = xmlhttp.responseText;
        var Recebe = xmlhttp.responseText.toString();
        
        //document.getElementById('D1_r').value = Recebe;  Se quiser analisar o que está vindo, pode usar esta linha de código
        //alert(Recebe);
        
        document.getElementById("Resposta_da_ESP").innerHTML = xmlhttp.responseText;
       
  }		  
}	
xmlhttp.send(null);	
        }

    window.onload = status;

	function liga(){ 
    var IP = "http://192.168.0.20/";

    url = IP + "lg_ext";
    xmlhttp.open("POST", url, true);
    xmlhttp.onreadystatechange = function(){
    
    if (xmlhttp.readyState==4){
            //document.getElementById("Resposta_da_ESP").innerHTML = xmlhttp.responseText;
            var Recebe = xmlhttp.responseText.toString();
            
            //document.getElementById('D1_r').value = Recebe;  Se quiser analisar o que está vindo, pode usar esta linha de código
            //alert(Recebe);
            
            document.getElementById("Resposta_da_ESP").innerHTML = xmlhttp.responseText;
           
      }		  
    }	
 xmlhttp.send(null);	
}

function desliga(){  
    var IP = "http://192.168.0.20/";

    url = IP + "dlg_ext";
    xmlhttp.open("POST", url, true);
    xmlhttp.onreadystatechange = function(){
    
    if (xmlhttp.readyState==4){
            //document.getElementById("Resposta_da_ESP").innerHTML = xmlhttp.responseText;
            var Recebe = xmlhttp.responseText.toString();
            
            //document.getElementById('D1_r').value = Recebe;  Se quiser analisar o que está vindo, pode usar esta linha de código
            //alert(Recebe);
            
            document.getElementById("Resposta_da_ESP").innerHTML = xmlhttp.responseText;
           
      }		  
    }	
 xmlhttp.send(null);	
}
	 
	function Limpar(){
	document.getElementById("Resposta_da_ESP").innerHTML=""; 
	}

	
</script>