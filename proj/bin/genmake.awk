{
    gsub(/\$\{TIMESTAMP\}/, strftime("%Y/%m/%d %H:%M:%S"))
    gsub(/\$\{RAD_HOME\}/, rad_home)
    print
}
