#!/usr/bin/env node
    
    // server-readFile.js
    
    var
      http = require('http'),
      fs = require('fs');
    
    function func1(str) {
      var res = '';
      for (var i = 0, l = str.length; i < l; i++) {
        res += str.charCodeAt(i);
      }
      return res;
    }
    
    function func2(str) {
      var res = 0;
      for (var i = 0, l = str.length; i < l; i++) {
        res += Math.sin(str.charCodeAt(i));
      }
      return '' + res;
    }
    
    http.createServer(function (req, res) {
      // Very simple and dangerous check
      var filename = req.url.replace(/\?.*/, '').replace(/(\.\.|\/)/, '');
      
      // Read file from disk
      fs.readFile(filename, 'utf8', function (err, filecontent) {
        if (err) {
          res.writeHead(404, {'Content-Type': 'text/plain'});
          res.end('File ' + filename + ' doesn\'t exist');
          return;
        }
        
        // Calculate checksum
        var hash = func2(func1(filecontent));
        
        // Write response
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end(hash);
      });
    }).listen(8666, "127.0.0.1");
    
    console.log(process.pid);
