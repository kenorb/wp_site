��    '      T  5   �      `  $   a     �     �     �     �     �  D   �  6   @  �   w  �     `   �  �        �     �  M         \     }     �  B  �  ?   �     )	     /	     4	  �   P	  �   �	    �
     �     �     �     �     �  /   
     :     L  5  b  r  �  )     �   5  �  #  /   �     �  %   �     $     >  )   Y  T   �  D   �  �     �   �  z   �  �   O     8     T  d   t  )   �             �  <  J   �  	          "   #  �   F  �   �  Q  �  +        J     \     v  %   �  ;   �     �       �  2  �  �  0   �   �   �                              %      "          
                 '                                          $             &          	                #                           !       'Authorization Header' rule disabled Authentication Authentication Type Pattern: Authorization Header Authorization header. Bad URL rewrite configuration By default HTTP authentication is only enabled for specific modules. Gallery can handle HTTP Basic authentication directly. Gallery can't access HTTP usernames and passwords and automated checks failed to find a cause.  Troubleshooting documentation is in the %sGallery Codex%s. Gallery can't access HTTP usernames and passwords.  You can still use your web server's authentication.  Troubleshooting documentation is in the %sGallery Codex%s. Gallery tried to logout but failed.  You should close the browser window to logout successfully. Gallery will prompt you to login with HTTP authentication when permission is denied.  HTTP authentication sends your client a realm to which your username belongs.  It's safe to leave the realm blank. HTTP Auth Settings HTTP Authentication Realm: If you're not automatically redirected, %sclick here to finish logging out%s. Login using HTTP authentication. Logout failed Missing HTTP Authorization PHP Path Info rewrite doesn't support the rule to fall back on passing HTTP usernames and passwords to Gallery.  You should uninstall and reinstall the URL rewrite module in the %sSite Admin Plugins option%s and choose either Apache mod_rewrite or ISAPI_Rewrite.  Troubleshooting documentation is in the %sGallery Codex%s. Pass the Authorization header to Gallery in a request variable. Reset Save Settings saved successfully Specify here a regular expression which the authentication type must match for authentication to proceed; for instance /Negotiate/ Specify here a regular expression which the username must match for authentication to proceed and a string with which to replace it.  See PHP %s documentation for more information. The URL rewrite rule to fall back on passing HTTP usernames and passwords to Gallery is disabled.  You should activate the HTTP auth 'Authorization Header' rule in the %sSite Admin URL Rewrite option%s.  Troubleshooting documentation is in the %sGallery Codex%s. URL rewrite module disabled Unknown Cause Use HTTP Authentication: Use Regular Expressions: Use Web Server Authentication: Use the authentication plugins for all modules: Username Pattern: Username Replacement: We can't fall back on passing HTTP usernames and passwords to Gallery because the URL rewrite module is disabled.  You should activate the URL rewrite module in the %sSite Admin Plugins option%s and choose either Apache mod_rewrite or ISAPI_Rewrite.  Troubleshooting documentation is in the %sGallery Codex%s. You may wish to trust only HTTP authentication types or HTTP usernames which match specified regular expressions - or HTTP usernames may not match your Gallery usernames; for instance if using %s authentication, the REMOTE_USER environment variable may be username@REALM.TLD.  In these cases, you may use regular expressions to filter authentication types and usernames. You must enter a valid regular expression Your web server may offer more comprehensive and more secure authentication.  If you configured your web server to authenticate requests to Gallery, you may choose to trust the username it reports in the REMOTE_USER environment variable. Project-Id-Version: Gallery: HTTP Auth 1.0.3
Report-Msgid-Bugs-To: gallery-translations@lists.sourceforge.net
POT-Creation-Date: 2006-11-16 13:57+0100
PO-Revision-Date: 2007-03-29 21:21+0200
Last-Translator: Óscar García Amor <oscar.garcia@moire.org>
Language-Team: Spanish <gallery-translations@lists.sourceforge.net>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
 'Cabecera de Autorización' regla deshabilitada Autenticación Tipo de Patrón de la autencicación: Cabecera de Autorización Cabecera de autorización. Mala configuración de reescritura de URL Por defecto la autenticación HTTP solo está habilitada para módulos específicos. Gallery puede manejar directamente la autenticación básica de HTTP Gallery no puede acceder a las contraseñas y nombres de usuario HTTP y las comprobaciones automáticas fallan al buscar la causa.  La documentación para la solución de problemas se encuentra en el %sGallery Codex%s. Gallery no puede acceder al los nombres de usuario y contraseñas HTTP.  Puedes seguir usando la autenticación de tu servidor web.  La documentación para la solución de problemas se encuentra en el %sGallery Codex%s. Gallery intentó cerrar sesión pero falló.  Deberías cerrar la ventana del navegador para cerrar la sesión con éxito. Gallery te preguntará para identificarte con la autenticación HTTP cuando se niegue el permiso.  La autenticación HTTP envía a tu cliente al dominio al cual tu nombre de usuario pertenezca.  Es seguro dejar el dominio en blanco. Configuración de HTTP Auth Dominio de Autenticación HTTP: Si no eres redirigido de forma automática, %shaz click aquí para cerrar sesión definitivamente%s. Identificarse usando autenticación HTTP. Fallo al cerrar sesión Autorización HTTP no encontrada La reescritura de Información de Ruta PHP no soporta la regla para pasar nombres de usuario y contraseñas HTTP a Gallery.  Deberias desinstalar y reinstalar el módulo de reescritura de URL en la %sopción de Plugins de la Administración del Sitio%s y elegir mod_rewrite de Apache o bien ISAPI_Rewrite.  La documentación para la solución de problemas se encuentra en el %sGallery Codex%s. Pasar la cabecera de Autorización a Gallery en una variable de petición. Reiniciar Guardar Configuración guardada con éxito Especifica aquí una expresión regular con la que el tipo de autenticación debe coincidir para que la autenticación proceda; por ejemplo /Negotiate/ Especifica aquí una expresión regular con la que el nombre de usuario debe coincidir para que que proceda la autenticación, y una cadena de caracteres con la cual se sustituirá. Mira la documentación de PHP %s para mas información. La regla de reescritura de URL para pasar nombres de usuario y contraseñas HTTP a Gallery está deshabilitada.  Debes activar la regla de HTTP auth 'Cabecera de Autorización' en la %sopción de Reescritura de URL de la Administración del Sitio%s.  La documentación para la solución de problemas se encuentra en el %sGallery Codex%s. Módulo de reescritura de URL deshabilitado Causa Desconocida Usar Autenticación HTTP: Usar Expresiones Regulares: Usar Autenticación del Servidor Web: Usar los plugins de autenticación para todos los módulos: Patrón de nombre de usuario: Reemplazo de nombre de usuario: No podemos pasar los nombres de usuario y las contraseñas a Gallery porque el módulo de reescritura de URL se encuentra deshabilitado.  Debes activar el módulo de reescritura de URL en la %sopción de Plugins de la Administración del Sitio%s y elegir mod_rewrite de Apache o bien ISAPI_Rewrite.  La documentación para la solución de problemas se encuentra en el %sGallery Codex%s. Puedes querer confiar solamente en los tipos de autenticación HTTP o los nombres de usuario HTTP que coincidan con las expresiones regulares especificadas - o los nombres de usuario HTTP pueden no coincidir con tus nombres de usuario de Gallery; por ejemplo si usando la autenticacion %s, la variable de entorno REMOTE_USER puede ser nombredeusuario@DOMINIO.TLD.  En estos casos, puedes usar expresiones regulares para filtrar tipos de autenticación y nombres de usuario. Debes especificar una expresión regular válida Tu servidor web puede ofrecer una autenticación mas extensa y segura. Si configuraste tu servidor web para autenticar las peticiones a Gallery, puedes elegir confiar en el nombre de usuario que se reporta en la variable de entorno REMOTE_USER. 