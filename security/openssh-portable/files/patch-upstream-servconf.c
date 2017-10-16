commit 7c9613fac3371cf65fb07739212cdd1ebf6575da
Author: djm@openbsd.org <djm@openbsd.org>
Date:   Wed Oct 4 18:49:30 2017 +0000

    upstream commit
    
    fix (another) problem in PermitOpen introduced during the
    channels.c refactor: the third and subsequent arguments to PermitOpen were
    being silently ignored; ok markus@
    
    Upstream-ID: 067c89f1f53cbc381628012ba776d6861e6782fd

diff --git servconf.c servconf.c
index 2c321a4a..95686295 100644
--- servconf.c
+++ servconf.c
@@ -1,5 +1,5 @@
 
-/* $OpenBSD: servconf.c,v 1.312 2017/10/02 19:33:20 djm Exp $ */
+/* $OpenBSD: servconf.c,v 1.313 2017/10/04 18:49:30 djm Exp $ */
 /*
  * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
  *                    All rights reserved
@@ -1663,9 +1663,9 @@ process_server_config_line(ServerOptions *options, char *line,
 		if (!arg || *arg == '\0')
 			fatal("%s line %d: missing PermitOpen specification",
 			    filename, linenum);
-		i = options->num_permitted_opens;	/* modified later */
+		value = options->num_permitted_opens;	/* modified later */
 		if (strcmp(arg, "any") == 0 || strcmp(arg, "none") == 0) {
-			if (*activep && i == 0) {
+			if (*activep && value == 0) {
 				options->num_permitted_opens = 1;
 				options->permitted_opens = xcalloc(1,
 				    sizeof(*options->permitted_opens));
@@ -1683,7 +1683,7 @@ process_server_config_line(ServerOptions *options, char *line,
 			if (arg == NULL || ((port = permitopen_port(arg)) < 0))
 				fatal("%s line %d: bad port number in "
 				    "PermitOpen", filename, linenum);
-			if (*activep && i == 0) {
+			if (*activep && value == 0) {
 				options->permitted_opens = xrecallocarray(
 				    options->permitted_opens,
 				    options->num_permitted_opens,
