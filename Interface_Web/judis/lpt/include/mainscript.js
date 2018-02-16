function solveProgram()
{
	var programTypeRadio = document.getElementsByName("programtype");
	var i;
	for(i = 0; !programTypeRadio[i].checked; i++);
	var programType = programTypeRadio[i].value;

	if(window.XMLHttpRequest) //para IE7+, Firefox, Chrome, Opera, Safari
		basicRequest = new XMLHttpRequest();
	else //para IE6, IE5
		basicRequest = new ActiveXObject("Microsoft.XMLHTTP");
	basicRequest.onreadystatechange = function() //função chamada sempre que o atributo readyState (estado da requisição http XML) do objeto de requisição é modificado
	{
		if(responseReadyToSend(basicRequest)) //quando a resposta estiver pronta e estiver tudo OK
			document.getElementById("outputsolution").value = basicRequest.responseText;
	}
	basicRequest.open("POST", "solve.php", true);
	basicRequest.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	basicRequest.send("inputprogram=" + encodeURIComponent(editor.getValue()) + "&programtype=" + programType);
}
	
function generateExample()
{
	if(window.XMLHttpRequest) //para IE7+, Firefox, Chrome, Opera, Safari
		exampleRequest = new XMLHttpRequest();
	else //para IE6, IE5
		exampleRequest = new ActiveXObject("Microsoft.XMLHTTP");
	exampleRequest.onreadystatechange = function() //função chamada sempre que o atributo readyState (estado da requisição http XML) do objeto de requisição é modificado
	{
		if(responseReadyToSend(exampleRequest)) //quando a resposta estiver pronta e estiver tudo OK
		{
			editor.setValue(exampleRequest.responseText);
			document.getElementById("outputsolution").value = tips;
		}
	}
	exampleRequest.open("GET", "randprog.php", true);
	exampleRequest.send();
}

function clearSolution()
{
	document.getElementById("outputsolution").value = "";
}

function responseReadyToSend(request)
{
	var output = document.getElementById("outputsolution");
	switch(request.readyState)
	{
		case 0:
			output.value = processing + "0%";
			return false;
			break;

		case 1:
			output.value = processing + "25%";
			return false;
			break;

		case 2:
			output.value = processing + "50%";
			return false;
			break;

		case 3:
			output.value = processing + "75%";
			return false;
			break;

		case 4:
			if(request.status == 200) return true;
	}

	if(request.status == 404)
	{
		alert(errorMessage);
		return false;
	}
}
