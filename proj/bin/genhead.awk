{
    gsub(/\$\{Name\}/, toupper(substr(name,1,1)) substr(name,2))
    gsub(/\$\{NAME\}/, toupper(name))
    gsub(/\$\{TIMESTAMP\}/, strftime("%Y/%m/%d %H:%M:%S"))
    print
}
