{
    inputs.nixpkgs.url = "nixpkgs/nixos-unstable";

    outputs = { self, nixpkgs, ... }:
        let pkgs = nixpkgs.legacyPackages.x86_64-linux;
        in {
        devShells.x86_64-linux.default = pkgs.mkShell {
            buildInputs = with pkgs; [
                gcc gnumake wxGTK31 pkg-config nlohmann_json
                xorg.libX11 xorg.libXtst xorg.libXi libvlc valgrind gtk3
            ];
        };
    };
}
