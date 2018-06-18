### instructions

```
git clone https://github.com/Bareflank/hypervisor
git clone https://github.com/boddumanohar/bfefi_memorymap.git 
mkdir build; cd build
cmake ../hypervisor -DDEFAULT_VMM=hello -DEXTENSION=../hello
make -j<# cores + 1>

sudo rm /boot/efi/bfloader.efi
sudo cp ./prefixes/x86_64-vmm-elf/bin/bfloader.efi /boot/efi

```




