# Content_Delivery_Network
In simple terms, a CDN helps websites deliver content in scale by storing recent versions that can be sent to
users without asking the server. So if 1000 users want to receive a PDF of a bank offer, all users should receive a copy from the CDN, 
and your server might get only 1-2 requests.

The idea behind a CDN cache is that whenever the 1st user requests a file X, we ask the server for it and store it.
Then, we set a freshness (e.g., 5 minutes). For any requests within 5 minutes, the cache just sends them the saved copy of the file. 
If a request comes after those 5 minutes, we ask the server again for the updated version. This reduces the number of requests that reach the server
since we serve the file straight from the CDN.

https://www.caiobatista.com/courses/uci/f22/ics45c/projects/p5-cdn/
