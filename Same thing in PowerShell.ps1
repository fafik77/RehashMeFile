Get-ChildItem -File -Path ./ | 
    Where { $_.Name -notmatch '^[0-9a-zA-Z]{32}\..*' } | 
    ForEach {
        # 1. Obliczamy hash dla konkretnego pliku
        $fileHash = (Get-FileHash $_.FullName -Algorithm MD5).Hash
        
        # 2. Tworzymy nową nazwę (hash + oryginalne rozszerzenie)
        $newName = $fileHash + $_.Extension
        
        # 3. Zmieniamy nazwę pliku
        Rename-Item -Path $_.FullName -NewName $newName
    }