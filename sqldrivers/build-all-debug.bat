cl /nologo /LDd /W3 /MTd /Zi /Ferlm_sql_oracle /I../proj /I../site/oracle8i/oci/include sql_oracle.c ../proj/lib/radutil.a ws2_32.lib ../site/oracle8i/oci/lib/msvc/oci.lib /DRAD_DLL
cl /nologo /LDd /W3 /MTd /Zi /Ferlm_sql_sybase /I../proj /I../site/sybase/include sql_sybase.c ../proj/lib/radutil.a ws2_32.lib ../site/sybase/lib/libct.lib ../site/sybase/lib/libcs.lib /DRAD_DLL
cl /nologo /LDd /W3 /MTd /Zi /Ferlm_sql_mysql /I../proj /I../site/mysql/include sql_mysql.c ../proj/lib/radutil.a ws2_32.lib ../site/mysql/lib/opt/libmysql.lib /DRAD_DLL
cl /nologo /LDd /W3 /GX /MTd /Zi /Ferlm_sql_msado /I../proj sql_msado.cpp ../proj/lib/radutil.a ws2_32.lib /DRAD_DLL
pause
