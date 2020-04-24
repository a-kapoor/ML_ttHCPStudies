# arg 1 : year
# arg 2 : CPmixing weight index
# arg 3 : training events bkg
# arg 3 : training events sig

root -b -l<<EOF
.L ttH_Classification_SimpleExample.C
ttH_Classification_SimpleExample("ttH_Classification_SimpleExample",$1,$2,$3,$4)
EOF
