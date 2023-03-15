# G4EasyAna
A series of code for the G4 data easy to convert to be the analysis format likes the experiment data, and do some easy analysis.
# !!!!!!Warning 2023/03/16 Update the file!!!!!!!!!
The new revised version has been created to address issues encountered in specific environments where the code cannot automatically include the script files (G4EventAna.C+ and BeamEasyStudy.C+) and to use the updated version of untuplizerv8_YuSiang.h, now named untuplizerv8_YuSiang01.h, which includes <TError.h> and the loop statement "while (fname = gSystem->GetDirEntry(dir))->while ((fname = gSystem->GetDirEntry(dir)))".
Add some functions to automatically create the directorys for file saving.
