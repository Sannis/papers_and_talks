#!/home/efimovov/.node_libraries/.npm/fibers/active/package/bin/node-fibers
    
    // server-readFile-and-node-sync.js
    
    var
      http = require('http'),
      fs = require('fs'),
      sync = require('sync');
    
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
    
    function func1_cb(str, cb) {
      var res = func1(str);
      
      cb(res);
    }
    
    function func2_cb(str, cb) {
      var res = func2(str);
      
      cb(res);
    }
    
    http.createServer(function (req, res) {
      // Very simple and dangerous check
      var filename = req.url.replace(/\?.*/, '').replace(/(\.\.|\/)/, '');
      
      // Start new fiber
      sync(function () {
        // Read file from disk
        var filecontent = fs.readFile.sync(fs, filename, 'utf8');
        var str = func1_cb.sync(null, filecontent);
        var hash = func2_cb.sync(null, str);
        
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.end(hash);
      }, function(err){
        // Error handling
        if (err) {
          res.writeHead(404, {'Content-Type': 'text/plain'});
          res.end(err);
        }
      })
    }).listen(8666, "127.0.0.1");
    
    console.log(process.pid);
