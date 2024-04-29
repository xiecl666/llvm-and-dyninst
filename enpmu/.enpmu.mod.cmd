cmd_/home/soc/xcl/llvm/enpmu/enpmu.mod := printf '%s\n'   enpmu.o | awk '!x[$$0]++ { print("/home/soc/xcl/llvm/enpmu/"$$0) }' > /home/soc/xcl/llvm/enpmu/enpmu.mod
