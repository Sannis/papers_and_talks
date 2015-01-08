#!/usr/bin/env node
    
    // server-readFileSync.js
    
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
      var start = Date.now();
      // Read file from disk
      try {
        var filecontent = fs.readFileSync(filename, 'utf8');
      } catch (e) {
        res.writeHead(404, {'Content-Type': 'text/plain'});
        res.end('File ' + filename + ' doesn\'t exist');
        return;
      }
      var read = Date.now();
      // Calculate checksum
      var hash = func2(func1(filecontent));
      var calc = Date.now();
      // Write response
      res.writeHead(200, {'Content-Type': 'text/plain'});
      res.end(hash);
      console.log((read - start) + ' ' + (calc - read));
    }).listen(8666, "127.0.0.1");
    
    console.log(process.pid);

