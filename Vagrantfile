# -*- mode: ruby -*-
# vi: set ft=ruby :

API_VERSION = '2'
BOX         = 'ubuntu/bionic64'
HOST_NAME   = 'icmp'
CPUS        = 2
MEM         = 2048

Vagrant.configure(API_VERSION) do |config|

  if Vagrant.has_plugin?("vagrant-timezone")
    config.timezone.value = :host
  end

  if Vagrant.has_plugin?("vagrant-vbguest") then
    config.vbguest.auto_update = false
  end

  config.ssh.insert_key = false

  config.vm.box = BOX

  config.vm.hostname = HOST_NAME

  config.vm.network :private_network, ip: '192.168.33.10'

  config.vm.provider :virtualbox do |v|
    v.check_guest_additions = false
    v.functional_vboxsf = false
    v.cpus = CPUS
    v.memory = MEM
    v.customize [ "modifyvm", :id, "--uartmode1", "disconnected" ]
  end

  config.vm.provision :shell, path: "bootstrap.sh"

  config.vm.synced_folder "../", "/vagrant", type: "nfs"

end
