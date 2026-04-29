import sqlite3
from bottle import route, run, debug, template, static_file, request, redirect, response

import random
import html
import secrets


@route('/static/<filename>')
def serve_static(filename):
	return static_file(filename, root='/home/student/safebank/static')



@route('/csp1')
def csp1():
    top='<html><body>'
    bot='</body></html>'
    mid='<p>Everything works here!</p>'
    myscript='<script>alert(5);</script>'
    name=request.GET.get('firstname','')
    return top+mid+myscript+name+bot

@route('/csp2')
def csp2():
    response.set_header("Content-Security-Policy","script-src 'self';")
    top='<html><body>'
    bot='</body></html>'
    mid='<p>XSS is stopped, but inline scripts from server also cannot execute</p>'
    myscript='<script>alert(5);</script>'
    name=request.GET.get('firstname','')
    return top+mid+myscript+name+bot


@route('/csp3')
def csp3():
    nonce = secrets.token_urlsafe(32)
    response.set_header("Content-Security-Policy","script-src 'nonce-"+nonce+"';")
    top='<html><body>'
    bot='</body></html>'
    mid='<p>The random nonce is '+nonce+' and it allows scripts from server to run</p>'
    myscript='<script nonce="'+nonce+'">alert(5);</script>'
    name=request.GET.get('firstname','')
    return top+mid+myscript+name+bot


@route('/csp4')
def csp4():
    nonce = secrets.token_urlsafe(32)
    response.set_header("Content-Security-Policy","script-src 'nonce-"+nonce+"';")
    top='<html><body>'
    bot='</body></html>'
    mid='<p>But be careful... bad enough code may still be vulnerable</p>'
    myscript='<script nonce="'+nonce+'">alert(5);</script>'
    name=request.GET.get('firstname','')
    badcode='<script nonce="'+nonce+'">'+name+'</script>'
    return top+mid+myscript+badcode+bot




debug(True)
run(host='0.0.0.0')
