$Point=for($i=0;$i -lt $InputData.count; $i++) {
 ([regex]::new('\w').Matches($InputData[$i])|?{$_.Success}|%{[pscustomobject]@{freq=$_.Value;X=$_.Index;Y=-$y+($InputData.count)}}}
$Point|select-object -Expand Value freq -Unique|%{
  $freq = $_
  $set = $Point|?{$_.freq -eq $freq}
  for($i=0;$i -lt $set.count - 2;$i++){
    $A=$set[$i]
    for($j=1;$j -lt $set.count - 1;$j++){
      $B=$set[$i+$j]
      [pscustomobject]@{x=$A.x-($B.x-$A.x);y=$A.y-($B.y-$A.y)}
      [pscustomobject]@{x=$B.x+($B.x-$A.x);y=$B.y+($B.y-$A.y)}
    }
  }
} |? {
  $_.x -ge 0 -and $_.x -lt $.InputData[0].count -and $_.y -ge 0 -and $_.y -lt $.InputData.count
} | Select-Object *