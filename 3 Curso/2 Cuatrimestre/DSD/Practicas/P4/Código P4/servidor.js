var http = require("http");
var url = require("url");
var fs = require("fs");
var path = require("path");
var socketio = require("socket.io");
var mimeTypes = { "html": "text/html", "jpeg": "image/jpeg", "jpg": "image/jpeg", "png": "image/png", "js": "text/javascript", "css": "text/css", "swf": "application/x-shockwave-flash"};
var MongoClient = require('mongodb').MongoClient;
var request = require('request');
var nodemailer = require('nodemailer');

var LUM_ACTUAL = 50
var TEMP_ACTUAL = 25
var HUM_ACTUAL = 50
var SUCIEDAD_ACTUAL = 0
var VIENTO_ACTUAL;

var UMBRAL_TEMP_MAX = 30
var UMBRAL_TEMP_MIN = 10
var UMBRAL_LUM_MAX = 20  
var UMBRAL_HUMEDAD_MAX = 20
var UMBRAL_HUMEDAD_MIN = 0
var UMBRAL_SUCIEDAD = 20
var UMBRAL_VIENTO = 2

var persiana = false  //false bajada - true subida 
var aireAcondicionado = false  //false apagado - true encendido
var roomba = false	//false apagado - true encendido
var humidificador = false //false apagado - true encendido
var ventana = true //false cerrrada - true abierta
var agente = true //Variable que apaga o enciende el agente domotico


function agenteDomotico()
{
	var mensajes = Array();
	if(agente)
	{

		if (TEMP_ACTUAL > UMBRAL_TEMP_MAX && !aireAcondicionado)
		{
			aireAcondicionado = true;
			mensajes.push("<p>El agente ha puesto el aire acondicionado porque la temperatura es muy alta</p>");
			if(!humidificador && HUM_ACTUAL < UMBRAL_HUMEDAD_MAX)
			{
				humidificador = true;
				mensajes.push("<p>El agente ha encendido el humidificador porque el aire acondicionado está en uso</p>");
			}	
		}
		if(TEMP_ACTUAL < UMBRAL_TEMP_MIN)
		{
			aireAcondicionado = true;
			mensajes.push("<p>El agente ha puesto el Aire Acondicionado en modo calor porque hace frío </p>")
			if(!humidificador && HUM_ACTUAL < UMBRAL_HUMEDAD_MAX)
			{
				humidificador = true;
				mensajes.push("<p>El agente ha encendido el humidificador porque el aire acondicionado está en uso</p>");
			}			
		}
		else if (TEMP_ACTUAL < UMBRAL_TEMP_MAX && TEMP_ACTUAL > UMBRAL_TEMP_MIN && aireAcondicionado)
		{
			aireAcondicionado = false;
			mensajes.push("<p>El agente ha apagado el aire acondicionado porque la temperatura está estable </p>");
		}


		if(aireAcondicionado && ventana)
		{
			ventana = false;
			mensajes.push("<p>El agente ha cerrado la ventana porque el Aire Acondicionado está en uso </p>");

		}else if (!aireAcondicionado && !ventana && VIENTO_ACTUAL < UMBRAL_VIENTO)
		{
			ventana = true;
			mensajes.push("<p>El agente ha abierto la ventana porque no está el Aire Acondicionado encendido, y no hay demasiado viento </p>");
		}

		if(VIENTO_ACTUAL > UMBRAL_VIENTO && ventana)
		{
			ventana = false;
			mensajes.push("<p>El agente ha cerrado la ventana porque la velocidad del viento es muy alta </p>");
		}

		if (LUM_ACTUAL < UMBRAL_LUM_MAX && !persiana)
		{
			persiana = true;
			mensajes.push("<p>El agente ha subido la persiana porque no hay mucha luminosidad </p>");
		}

		if (LUM_ACTUAL > UMBRAL_LUM_MAX && persiana)
		{
			persiana = false;
			mensajes.push("<p>El agente ha bajado la persiana porque hay demasiada luz en la habitación</p>");
		}

		if(SUCIEDAD_ACTUAL > UMBRAL_SUCIEDAD && !roomba)
		{
			roomba = true;
			mensajes.push("<p>Se ha activado la roomba </p>");

		}else if (SUCIEDAD_ACTUAL < UMBRAL_SUCIEDAD && roomba)
		{
			roomba = false;
			mensajes.push("<p>La roomba ha terminado de limpiar</p>")
		}

		if(HUM_ACTUAL < UMBRAL_HUMEDAD_MIN && !humidificador)
		{
			humidificador = true;
			mensajes.push("<p>El agente ha encendido el humidificador porque el ambiente está muy seco</p>");

		} else if(HUM_ACTUAL > UMBRAL_HUMEDAD_MAX && humidificador)
		{
			humidificador = false;
			mensajes.push("<p>El agente ha apagado el humidificador porque el ambiente está muy húmedo</p>");
		}
	}
	else{
		mensajes.push("<p>El agente esta desactivado</p>");
	}
	return mensajes;
}

var httpServer = http.createServer(
	function(request, response) {
		var uri = url.parse(request.url).pathname;
		if (uri=="/") uri = "/sensores.html";
		var fname = path.join(process.cwd(), uri);
		fs.exists(fname, function(exists) {
			if (exists) {
				fs.readFile(fname, function(err, data){
					if (!err) {
						var extension = path.extname(fname).split(".")[1];
						var mimeType = mimeTypes[extension];
						response.writeHead(200, mimeType);
						response.write(data);
						response.end();
					}
					else {
						response.writeHead(200, {"Content-Type": "text/plain"});
						response.write('Error de lectura en el fichero: '+uri);
						response.end();
					}
				});
			}
			else{
				while (uri.indexOf('/') == 0) uri = uri.slice(1);
				var params = uri.split("/");
				if (params.length >= 2) {
					console.log("Peticion REST: "+uri);
					response.writeHead(200, {"Content-Type": "text/html"});
					response.end();
				}
				else {
					console.log("Peticion invalida: "+uri);
					response.writeHead(200, {"Content-Type": "text/plain"});
					response.write('Error 404 Not Found\n');
					response.end();
				}
			}
		});		
	});


MongoClient.connect("mongodb://localhost:27017/", {useNewUrlParser: true, useUnifiedTopology: true},function(err, db){
	httpServer.listen(8080);
	console.log("Servicio HTTP iniciado");
	var io = socketio(httpServer);
	var users = new Array();
	var dbo = db.db("pruebaBaseDatos");

	dbo.collection("registroCambios").deleteMany({});

	dbo.createCollection("registroCambios", function(err, collection){

	io.sockets.on('connection', function(u){
		users.push({address:u.request.connection.remoteAddress, port:u.request.connection.remotePort});		
		obtenerRegistro();

		io.emit('usuariosActivos',users);

		u.emit('valoresSensores', {
			temperatura: TEMP_ACTUAL,
			luminosidad: LUM_ACTUAL,
			humedad: HUM_ACTUAL,
			suciedad: SUCIEDAD_ACTUAL,
			viento: VIENTO_ACTUAL,
			estadoAC: aireAcondicionado,
			estadoPersiana: persiana,
			estadoAgente: agente,
			estadoRoomba: roomba,
			estadoVentana: ventana,
			estadoHumidificador: humidificador, 
			mensajesAgente : '' 
		});

		u.on('AC',function(){
			aireAcondicionado = !aireAcondicionado;
			var mensaje;
			if(aireAcondicionado) 
				mensaje = 'El usuario ha encendido el aire acondicionado manualmente' 
			else 
				mensaje = 'El usuario ha apagado el aire acondicionado manualmente'
			actualizarValoresUsuarios(mensaje);
			obtenerRegistro() //Actualizamos el registro 
		})

		u.on('persiana',function(){
			persiana = !persiana;
			if(persiana) 
				mensaje = 'El usuario ha subido la persiana manualmente' 
			else 
				mensaje = 'El usuario ha bajado la persiana manualmente'
			actualizarValoresUsuarios(mensaje);
			obtenerRegistro() //Actualizamos el registro 
		})

		u.on('ventana',function(){
			ventana = !ventana;
			if(ventana) 
				mensaje = 'El usuario ha abierto la ventana manualmente' 
			else 
				mensaje = 'El usuario ha cerrado la ventana manualmente'
			actualizarValoresUsuarios(mensaje);
			obtenerRegistro() //Actualizamos el registro 
		})

		u.on('roomba',function(){
			roomba = !roomba;
			if(roomba) 
				mensaje = 'El usuario ha encendido el roomba manualmente' 
			else 
				mensaje = 'El usuario ha apagado el roomba manualmente'
			actualizarValoresUsuarios(mensaje);
			obtenerRegistro() //Actualizamos el registro 
		})

		u.on('humidificador',function(){
			humidificador = !humidificador;
			if(humidificador) 
				mensaje = 'El usuario ha encendido el humidificador manualmente' 
			else 
				mensaje = 'El usuario ha apagado el humidificador manualmente'
			actualizarValoresUsuarios(mensaje);
			obtenerRegistro() //Actualizamos el registro 
		})


		u.on('obtenerValoresSensores',function(data){
			if(data.sensor == 'temperatura')
			{
				TEMP_ACTUAL = data.valor;

			}else if(data.sensor == 'luminosidad')
			{
				LUM_ACTUAL = data.valor;

			}else if(data.sensor == 'suciedad')
			{
				SUCIEDAD_ACTUAL = data.valor;

			}else if(data.sensor == 'humedad')
			{
				HUM_ACTUAL = data.valor;
			}
			var mensajes = agenteDomotico();
			actualizarValoresUsuarios(mensajes);
			dbo.collection("registroCambios").insertOne(data, {safe:true}, function(err, result) {});
			obtenerRegistro()
		})

		u.on('obtenerRegistro',function(){
			obtenerRegistro();
		});


		u.on('estadoAgente',function(){
			agente = !agente;
			var mensajes = agenteDomotico();
			actualizarValoresUsuarios(mensajes);
		})


		u.on('disconnect',function(){
			console.log("El cliente "+u.request.connection.remoteAddress+" se va a desconectar");
			users = desconectar( users, {address:u.request.connection.remoteAddress, port:u.request.connection.remotePort} );
			io.emit('usuariosActivos',users);
		});

		u.on('enviarCorreo',function(){
			console.log("El cliente "+u.request.connection.remoteAddress+" ha enviado un correo");
			enviarCorreo();
		});


		function obtenerRegistro()
		{
			dbo.collection("registroCambios").find().toArray(function(err, results){
				io.emit('obtenerRegistro',results);
			});
		}


		});

		function desconectar ( arr, item ) {
			var i = -1;

			for(var j = 0; j < arr.length; j++)
				if(arr[j].port == item.port)
					i = j;

			if ( i != -1 ) {
				arr.splice( i, 1 );
			}

			return arr;
		}

		function actualizarValoresUsuarios(mensajes)
		{
			io.sockets.emit('valoresSensores', {
				temperatura: TEMP_ACTUAL,
				luminosidad: LUM_ACTUAL,
				humedad: HUM_ACTUAL,
				suciedad: SUCIEDAD_ACTUAL,
				viento: VIENTO_ACTUAL,
				estadoAC: aireAcondicionado,
				estadoPersiana: persiana,
				estadoAgente: agente,
				estadoRoomba: roomba,
				estadoVentana: ventana,
				estadoHumidificador: humidificador,  
				mensajesAgente : mensajes
			});
		}

	

	//Función para obtener la velocidad del viento mediante Api
	let apiKey = '880d4da8b85d708ec30cd7cc793e86e1';
	let ciudad = 'Granada';
	let url = 'http://api.openweathermap.org/data/2.5/weather?q='+ciudad+'&lang=es&units=metric&appid='+apiKey
	
	function obtenerVientoApi(){
	request(url, function (err, response, body) {
			if(err){
				console.log('error:', err);
			} else {
				let info= JSON.parse(body);
				VIENTO_ACTUAL = info.wind.speed;
				var mensajes = agenteDomotico();
				actualizarValoresUsuarios(mensajes);
			}
		});
	}
	//Se obtiene al principio de ejecución y luego se actualiza cada 20 segundos.
	obtenerVientoApi();
	setInterval(()=>obtenerVientoApi(),20000);

	//Función para enviar correos mediante Api
	function enviarCorreo(){
		var transporter = nodemailer.createTransport({
			service: 'gmail',
			auth: {
			  user: 'ravenwraith00@gmail.com',
			  pass: 'aprobarDSDcon10'
			}
		  });
		  
		  var mailOptions = {
			from: 'ravenwraith00@gmail.com',
			to: 'rullencastro@gmail.com',
			subject: 'Informe del Sistema IoT',
			text: 'Temperatura (ºC) = '+TEMP_ACTUAL+' ||| Aire Acondicionado: '+aireAcondicionado+
			'\n\nLuminosidad (%) = '+LUM_ACTUAL+' ||| Persiana : '+persiana+
			'\n\nHumedad (%) = '+HUM_ACTUAL+' ||| Estado Humidificador: '+humidificador+
			'\n\nSuciedad (%) = '+SUCIEDAD_ACTUAL+' ||| Estado Roomba: '+roomba+
			'\n\nVelocidad viento (m/s) = '+VIENTO_ACTUAL+' ||| Estado ventana: '+ ventana+
			'\n\nEstado del agente domótico : '+agente
		  };
		  
		  transporter.sendMail(mailOptions, function(error, info){
			if (error) {
			  console.log(error);
			} else {
			  console.log('Email enviado: ' + info.response);
			}
		  });
	}

	});

});

	


