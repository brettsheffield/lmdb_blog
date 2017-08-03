# lmdb_blog

A very simple blog, using lmdb for the backend.

Designed to work with gladd + libgladdb.


## Installation

### Install lmdb_blog

```
make
make install
```

### Install libgladdb

https://github.com/brettsheffield/libgladdb

First, ensure lmdb is installed on your server, then configure and install libgladdb in the usual manner.

```
git clone https://github.com/brettsheffield/libgladdb.git
cd libgladdb/src
./configure
make
make install
```


### Install gladd

https://github.com/brettsheffield/gladd

```
git clone https://github.com/brettsheffield/gladd.git
cd gladd/src
./configure --with-openssl
make
make install
```

Follow the instructions for creating a LetsEncrypt certificate:

https://github.com/brettsheffield/gladd/blob/master/LETSENCRYPT.md


#### Example gladd.conf

The following gladd.conf is sufficient for a TLS-enabled blog:

```
debug 1
port 443
ssl 5
ssl-key /etc/letsencrypt/live/blog.example.com/privkey.pem
ssl-cert /etc/letsencrypt/live/blog.example.com/fullchain.pem
url_default /index

db lmdb0 lmdb * /var/cache/blog/blog.mdb

document_root /home/user/public_html

url static GET /css/* css/
url static GET /media/* media/

url keyval GET /* lmdb0 $0

template keyval GET /* lmdb template.base
```

Start gladd with:

`gladd start`

### Create an xslt template

Something like this will do for a start:

```xslt
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
 <xsl:output method="html" encoding="UTF-8" indent="yes" />
 <xsl:template match="@*|node()">
  <xsl:text disable-output-escaping='yes'>&lt;!DOCTYPE html&gt;&#xa;</xsl:text>
          
  <html lang="en">
   <head>
    <title><xsl:value-of disable-output-escaping="yes" select="//h1"/> - My Blog</title>
    <link rel="stylesheet" href="/css/screen.css" />
   </head>
   <body>

    <div class="blogpost">
     <!--- blog post here --->
     <xsl:copy-of disable-output-escaping="yes" select="child::node()" />
    </div>

   </body>
  </html>
 </xsl:template>
</xsl:stylesheet>

```

This will use the contents of the first H1 tag as the `<title>`.

### Prepare lmdb

Ensure the lmdb database directory is readable and writable by you and gladd.

eg `chmod 0700 /var/cache/blog/`


### Write a blog post

Create a html file, say post1.html, and feed it into lmdb like so:

`blog_write index < post1.html`

NB: HTML must be well formed.  Any mismatched tags will result in a 500 server error.  Let's all write well-formed HTML :-)  HTML must be contained within a single outer pair of tags.  I suggest `<html> </html>`.


You can output it again with:

`blog_read index`

or redirect it to a file for re-editing:

`blog-read index > /tmp/index.html`

More posts can be added like so:

`blog_write my-new-post < some.html`

and this will be available at https://blog.example.com/my-new-post

### Test

Having done all that, you should be able to visit https://blog.example.com/ and view your new blog.
