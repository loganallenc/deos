#!/home/deos/venv/bin/python
# -*- coding: utf-8 -*-

import web
from htmlmin.minify import html_minify

app = web.auto_application()

class home(app.page):
    path = '/'
    def GET(self):
        return 'hello, world!'

if __name__ == "__main__":
    web.wsgi.runwsgi = lambda func, addr=None: web.wsgi.runfcgi(func, addr)
    app.run()
