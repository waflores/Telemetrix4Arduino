{
  description = "Telemetrix Cross Platform Arduino I2C Interface";

  nixConfig.bash-prompt-suffix = "telemetrix> ";
  inputs = {
    # keep-sorted start

    blueprint.inputs.nixpkgs.follows = "nixpkgs";
    blueprint.url = "https://github.com/numtide/blueprint/archive/56131e8628f173d24a27f6d27c0215eff57e40dd.tar.gz";
    devshell.inputs.nixpkgs.follows = "nixpkgs";
    devshell.url = "https://github.com/numtide/devshell/archive/255a2b1725a20d060f566e4755dbf571bbbb5f76.tar.gz";
    nixpkgs.url = "https://github.com/NixOS/nixpkgs/archive/7f817b8455a1e9a944ab4392cc16c3ca36dbc83e.tar.gz"; # master - 2026-04-17
    treefmt-nix.inputs.nixpkgs.follows = "nixpkgs";
    treefmt-nix.url = "https://github.com/numtide/treefmt-nix/archive/790751ff7fd3801feeaf96d7dc416a8d581265ba.tar.gz";
    # keep-sorted end
  }; # End inputs

  outputs =
    inputs:
    inputs.blueprint {
      inherit inputs;
      systems = [ "x86_64-linux" ];
      nixpkgs.config.allowUnfree = true;
      nixpkgs.config.allowUnfreePredicate =
        pkg: builtins.elem (inputs.nixpkgs.lib.getName pkg) [ "vscode" ];
    };
}
