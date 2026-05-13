# Using mkShell from nixpkgs
{
  pkgs ? import <nixpkgs> { },
  ...
}:
let
  teensy_loader_cli = pkgs.teensy-loader-cli.overrideAttrs (old: rec {
    installPhase = old.installPhase + ''
      ln -sf $out/bin/teensy-loader-cli $out/bin/teensy_loader_cli
    '';
  });
in
pkgs.mkShell {
  name = "tools";
  packages =
    with pkgs;
    [
      # keep-sorted start
      avrdude
      bashInteractive
      nix-fast-build
      nix-output-monitor
      nix-tree
      platformio
      tree
      # keep-sorted end
    ]
    ++ [ teensy_loader_cli ];
  shellHook = ''
    export PLATFORMIO_CORE_DIR=$PWD/.platformio
  '';

}

# https://github.com/numtide/blueprint/blob/main/docs/content/guides/configuring_direnv.md
