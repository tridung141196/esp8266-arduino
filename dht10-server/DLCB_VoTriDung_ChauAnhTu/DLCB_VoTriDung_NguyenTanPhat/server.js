//---------------------------------------------------------------------------------------------
var fs = require('fs');
var url = require('url');
var http = require('http');
var querystring = require('querystring');
var db = []; //database
var mode = '00';
//---------------------------------------------------------------------------------------------
// function gửi yêu cầu(response) từ phía server hoặc nhận yêu cầu (request) của client gửi lên
function requestHandler(request, response) {
 
// Giả sử địa chỉ nhận được http://192.168.1.7:8000/update?temp=30&humd=40
 
var uriData = url.parse(request.url);

 
var pathname = uriData.pathname;
// /update?
 
var query = uriData.query;
// temp=30.5&hum=80
 
var queryData = querystring.parse(query); // queryData.temp = 30.5, queryData.humd = 40
 
//-----------------------------------------------------------------------------------------
 
if (pathname == '/update') {
 
var newData = {
 
temp: queryData.temp,
 
humd: queryData.humd,
 
time: new Date()
 
};
 
db.push(newData);
 
console.log(newData);
 
response.end();
 
//-----------------------------------------------------------------------------------------
}else if (pathname == '/set') {

mode = queryData.mode;
console.log(mode);
}else if (pathname == '/getmode') {

response.writeHead(200, {

'Content-Type': 'text/plain'
});
response.end(mode);

} else if (pathname == '/get') {
 
response.writeHead(200, {
 
'Content-Type': 'application/json'
 
});
 
response.end(JSON.stringify(db));
console.log(db);
db = [];

 
//-----------------------------------------------------------------------------------------
 
} else {
 
fs.readFile('./new.html', function(error, content) {
 
response.writeHead(200, {
 
'Content-Type': 'text/html'
 
});
 
response.end(content);
 
});
 
}
 
//-----------------------------------------------------------------------------------------
}
var server = http.createServer(requestHandler);
server.listen(8000);
console.log('Server listening on port 8000');
