[CmdletBinding()]
param()
Process {
    $needles | ForEach-Object -begin { 
        $percent_current = 0
        $percent_total = $needles.Count
        $progress = @{
            Status = 'Testing inputs'
            Activity = 'AdventOfCode 2024-19.1' 
        } 
        $total = 0
        $dictionary = @{}
    } -end { 
        Write-Progress @progress -Completed 
        Write-Host "Total solutions found: $total" -ForegroundColor Magenta
    } -process {
        Write-Progress @progress -CurrentOperation $_ -PercentComplete ($percent_current / $percent_total * 100)
        $parts = [System.Collections.ArrayList]@()
        Write-Host $_ -ForegroundColor Green
        aoc2024_19_2_v2 -needle $_ -layers $layers -parts ([ref]$parts) -dictionary ([ref]$dictionary) | Foreach-Object -begin { $counter = 1} -process {
            ("$([char]27)[2K$([char]27)[0G> Total: {0:d12} - Current: {0:d8} - $($_.Solution -join ', ')" -f $total++,$counter++) | Write-Host -NoNewline
        }
        Write-Host ''
    }

}
Begin {
    function aoc2024_19_2_v2 {
        param(
            [Parameter(Mandatory)][string]$needle,
            [Parameter(Mandatory)]$layers,
            [Parameter(Mandatory)][ref]$parts,
            [Parameter(Mandatory)][ref]$dictionary
        )
        $max = ($layers.ItemLength | Measure-Object -Maximum | Select-Object -ExpandProperty Maximum )
        $min = ($layers.ItemLength | Measure-Object -Minimum | Select-Object -ExpandProperty Minimum )
        $lengths = $layers.ItemLength | Sort-Object -Unique
        
        Write-Warning "---- Needle: $needle"

        $all_matches = $lengths | Where-Object {
            # Only consider lengths that are less than or equal to the needle length
            $_ -le $needle.Length
        } | ForEach-Object {
            # Look for matches in the layers for the current length
            $needle_length = $_
            $needle_substring = $needle.Substring($needle.Length - $needle_length)
            
            #Write-Debug "Length: $needle_length, Substring: $needle_substring"
            
            $layers | Where-Object { 
                $_.ItemLength -eq $needle_length 
            } | ForEach-Object {
                # Return the matches, if any
                $_.Items | Where-Object {
                    $_ -eq $needle_substring
                }
            }
        }
        if(-not $all_matches) {
            # Write-Verbose "No matches found for needle: $needle"
            # No matches found, return false
            #$false | Write-Output
        } else {
            Write-Warning "NEEDLE     : $needle"
            Write-Warning "ALL MATCHES: $($all_matches -join ',')"
            $all_matches | Foreach-Object {

                # Process each match found
                $needle_substring = $_
                $remaining_needle = $needle.Substring(0, $needle.Length - $needle_substring.Length)

                # Write-Debug "Needle: $needle, Remaining: $remaining_needle, Match: $needle_substring"

                # Add the value to the list
                $parts.Value.Add($needle_substring) | Out-Null

                if($remaining_needle.Length -eq 0) {
                    # The whole needle was processed, we have a solution
                    $cloned_parts = $parts.Value.Clone()
                    $cloned_parts.Reverse()
                    [pscustomobject]@{Solution = $cloned_parts} | Write-Output 

                    <## Feed the dictionary
                    for ($i = 2; $i -le $cloned_parts.Count; $i++) {
                        $key = ($cloned_parts | Select-Object -Last $i) -join ''
                        $value = [System.Collections.ArrayList]@($cloned_parts | Select-Object -Last ($i))
                        if (-not $dictionary.Value.ContainsKey($key)) {
                            $dictionary.Value[$key] = [System.Collections.ArrayList]@()
                        }
                        if (-not $dictionary.Value[$key].Contains($value)) {
                            $dictionary.Value[$key].Add($value)
                            Write-Warning "Key: $key, Value: $($value -join ',')"
                        }
                    }#>

                    # $true | Write-Output
                } else {
                    # Process the remaining needle recursively
                    aoc2024_19_2_v2 -needle $remaining_needle -layers $layers -parts $parts -dictionary $dictionary | Write-Output
                }

                # Remove the value from the list
                $parts.Value.RemoveAt($parts.Value.Count - 1) | Out-Null
            }
        }
    }


    if($false) {
        # TEST DATA
        $patterns = @"
r, wr, b, g, bwu, rb, gb, br
"@
        $needles = @"
brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb
"@
    } else {
        # REAL DATA
        $patterns = @"
uwbr, bbgrb, wwbg, gugurur, rgurw, rbgw, bw, wurbru, bgrurw, gbw, bwgg, ubwg, wrgb, bwg, rwgug, bbgb, grbrbuug, ruw, grrrbu, ubwurbwu, rbg, buu, bru, rwugubg, brgwu, wr, uwb, wgwu, rubgwu, rrw, bwwrb, rrr, uww, gbgwrww, gbb, uugw, wurgr, wgbwbrur, grwbg, gwrbuw, wgu, wrr, ug, rbr, guur, gurru, urgwggw, wuwbwg, bwggrwgw, brurww, wbr, buww, bgrb, bbb, rgr, bgbbwbu, bububub, wbugu, gwgub, gubbwwrb, rrgwuuwu, uggb, rgbg, ubgbrw, rrrg, rr, gbuwb, wwbugwb, rbwbr, wbrrb, rug, b, bwwrwr, gug, bwrw, rbu, r, bgu, uub, bwb, ubwuuw, uuur, wgur, urw, gbbw, uuuru, ur, wburb, gwuuw, rbb, wuu, gru, urb, wb, bbwb, grgugw, rgruuur, rrwbwrg, uubru, ubrw, rbbgrgr, br, wub, ww, guu, gur, uug, bww, ubrwbgu, brwu, wrbgub, ggwwgwb, uuwuugrb, rwgrw, ggurr, gbg, rrwb, gbrbgr, bbruu, rrbrbw, wrurug, ruuw, wg, bwwwwb, bubg, ubr, bwu, uugub, ruuuubb, rggw, urr, urwgr, rww, wwggu, gbbwgw, ubggu, wbb, uugr, uubgu, uwg, rwwuru, uuurb, buuu, wbru, ubwr, urg, urrwrwgb, rbubb, uw, brrg, bgb, gwrguu, bgwu, bubwbg, rb, grb, uugubb, gww, bggbru, gwb, grwwwugb, gwggbg, bbg, ubrg, rgu, gwguu, ruwrwb, gggwb, bbbbug, ruwrurbw, rrbww, rwbww, grru, wwrwwub, guwurb, ugwbg, wgruuwwb, rg, uubgww, ruu, gbbbrbb, ggruu, gwgbwu, buw, uur, uuurbgr, uu, gwwrwgb, brrr, uwuw, guw, rwwr, gwuguub, wgb, brwwr, brub, uurg, ugb, rguub, brwbg, ruuuu, bggbgw, wrgwwwg, rrgw, uurrg, wwuwu, gugg, wwub, wuw, uuu, wugruu, rrgb, uwr, rbrwwb, grr, rrrb, bgg, ruwr, ggrurwr, ugg, bwr, rguwru, wgub, ugr, wrwu, gwbw, bwbur, wgbbr, rrrw, grwrgg, rgb, ugu, wwg, wugur, ggu, ubrwwgbu, uwub, urubwug, ugw, wguub, wbubugw, gugwwu, rwb, ubuu, gbu, wugbg, gbbu, rrg, brbr, urrg, guwub, bbr, wbbuburr, wwburwb, grbugbu, uuwbrw, wubr, wbbggww, ubub, ruru, grwbgbb, bbw, uru, wgg, ubrwu, rw, wurbu, uwrwb, rrb, rugggr, rgrwr, wrw, ggw, grgugu, uwug, rur, wruuw, bugbur, bb, uwggrrb, gbgr, rgbrw, bur, rbw, bub, wrb, rrgr, wrug, gr, uubw, gbr, rguuwbb, buubg, ugrbrwr, gubu, ru, wgr, ubggb, gwuu, rwgru, rwwrwg, grrr, gwu, guuuwww, ggugug, ubwrgbww, rgwbggu, brr, uubbr, wgwbw, ub, rrbru, uwuuur, rgwr, rruw, urgguw, urwb, uggr, bu, brwrw, gggub, uurb, buuw, ggb, wbrbgu, gurb, wuwbu, grg, buubbg, ggrubb, urwbwrb, uwugrgu, rurrw, ubw, wrg, u, www, bgr, wrbgw, wbg, rub, wwu, ubrgwg, bwuw, brb, ggr, bwrb, wggb, ubb, wrrrgw, urubrgwg, bbgr, uwwr, rwg, buwubw, bgw, brg, w, rubb, bwwwbb, ggg, bbbw, gbguw, guuw, grggr, wur, wug, grwwgrw, wbbwu, rru, bgbwgr, brw, gwg, uubwwur, wrwgw, wubwb, ugubrb, burb, ggwrr, rwbg, bwgugw, gbgwgwgr, uwgb, wu, gugww, wwuugrwb, rugu, wgw, rwub, uuwwbg, grrg, uuw, gu, gwgb, uwwur, rwbbbbb, gbur, uwugrwgg, rrgg, buwg, rwr, bwuwrgb, ubu, bgbrguwb, rbru, gw, bbbbg, ubuwuu, bbu, rgg, rwu, wbw, ggwbru, guwb, wwr, wuubg, ubrww, rwruru, urbww, gwrr, wrgwbu, bwbrugu, wwrww, grw, rgw, wwb, wguuwrw, wrgrg, wgug, uwwg, bbubwbru, wrwbu, gub, bwbg, wgrwr, gg, burbrbw, uggu, rgbgbw, gwr, rgrwbw, gguuur, urbw, wwrgw, wrggu, gbug, ubug
"@
        $needles = @"
bwwbrubububwugbugruuwbbwbbbbwuwruuuwbuuwbwwwgwgr
wbwrgruwgubuwugbbwurwuurgrururbugbgbuggrugbu
wgrggwgruubbgbgurwrbgggwbuwwruburwrbrwwgrubg
wgrbbuwgbwburbrrrubburubruwugbbbwuuwwruwbgbrwbbwuruwb
wwrgwugrgbuwwwububrrrggwgugbwguwuurrgrrwggbrugbugrbur
rguwbbggwbbuggugwwbgbbwwrgbbbrwbgbbwrurwrggugrwgrwr
rguguwbgbwrbrrguuwwubrrugrruurgbrrbrguuwgwgggrbw
guuubururbgbwwbwrbgwwbgbrwbuwurgubggwwbwrb
bburbggbbwgrbbuwrbguugbruwbgwrwbwgrwgrwgbugbuwggbbuurrb
guwbgbbbuwrwruuwgbrwugwwuruuwrruurubbwwgrwg
grgbgurrwguwrgguugwbugbbgrbbbbrgwrugurrbwr
uuwgwgbrwrbbrruwwrwugbguurwgbruuubwbbrrrubugr
ggbbbwwuuggwubbbubggggrrbwwgwgbwguwuuwgubrwrrwwgr
gurgugbbwgbbggurrbbgwgubgwbbwwruuuuuwbbbwwguwgrwuuwrwruurr
wubuuwurwuurbrrbugbuurgwuwgugugurrbwurbgwwuguwuurww
bbgrwguugbrgwbggwgbwwwwggrguwbgwguubwgwrbuwrugrrugrrugwr
rrwubgurwgugggbrgbguwgrugugrubugguuubggugw
wbggguugwrgwrbrrubburbwwgburwrubwbwbrgrrubbwwgwr
rwbgugwrrbbuwuwrggubwwwrbbrgrruwrwggbggwuggruguurruurgbwuw
wrbuwrrbrbbrbururwrwbubbrrrwrugbwgggbrguurgbrbwrwbrrwwwu
ugwrwbbruggwrwwbwurrrrurbgbwuurrgburwbwurubwgbwubg
ruggbbububbbwwrrbbbbrwbwwrrgbgrwuwwwrgwbgrbrruurrgwurbrguu
uugbuuugggruwgwuuwuugrbbbbwbuwgbwgbwwrwrwrbw
wgwrwrbbgrugrbbrurububwwrbbuugggrugugrrubg
wrbrbbwgrggugbrggggwuwurbwururbburwuwwwugbwgbuubrubg
bggwwurrwrbrwgwuwrbbrwruuugwwuubgugwgbrubbrubg
gguugguuguuwwwrbuuwrbuuggburrrggwgubbrurwrugu
gwugrgugurwgggurgwrbbrgrburwrrwbrwgrwuggbuuwuubwbgbgbb
wuwwwbwgbgbgbwrgwrruubwgrbgbugwwggbgwruwwwburwrrrwururu
ubrububwrrwurrwrurugrwbruggrbugrrrwguuggbbrgbb
wwugrwwrbbwbwwurgbwgggggwruwgugbuggurwrbbbgwugbwuururw
wgbgrgwugurgwwugwururwubbbuurwbbbbrwubrugurwbbrubbguwu
wwrwrrggururuurgrrrgrbrrwwuugrwbgbwrgbrurugbbbwrgbbwbu
rwwrrbgbwugubwurbubrrrubugwurrbbruurwruwrruwbubrw
gwbugburwwgruuwwbwbbrrugubgggbburububburubbruwguubg
rrubrbbrruubwbwgbwggbwuwggwgwurugrrrrwrurgbwbbwuuuwgubg
brgrgwguwwuwurwbbruwrwwruwrrwgwwrgggrubwgrbggrrgb
bgrurwbguuruwuugurbwwgrruuububbbbgbrbbwubrbbwrugrrgbw
rggwbrwrwggguwgbubgrwgubbggrurgrubrrwbwbrrbggbwbbb
uguwgrruwbbwbuguuubguguuuwrubgbubwbuugwbuwbgbrbgrr
rgrgwrrbbwwbrbrwbbwwbrbwuwrbbgbbrgbuubgubbgbb
wurgbbuggguwgwwwuwbrwbgwbrgurwruwuwgurwubggbggbg
uurrbgruwrbbuwubuwugwbbrwrurugrgwwrbbrrrrwrurbbrbbw
wuuwugbbugbbuurugrbbwbgbwguguwugbbrwbwugwgbu
gbugbwgbbgurwbbugwbggrgwwuugrgrgbuurwwbgwurbrwrbgrwwurbru
urbgwrggwbbuurrburbrbgbbgrgrubbwugurbwuwwuruwurgw
gbwwwbrubgwbbbwrgbuurwwwrrrgubgbruwwbugbrrwguwgr
urwruugwrbbwgbwrburrugwbwwgbubbbguwgrubg
gwwbwuwbwbgrgrrrrgururrrrugrgrrrrrrwububwgbwrurubg
rgrbrbuggwwurwguggwugbrbrgurrggwrrubwggwgubgurbggburw
uuubgrgwbwgrurggwwbuwurwubrgbbgbwrgrrwrwwgwwbgbbrrwgwbrbgubg
bgrrrbbwrbbbwwggwurbwbwubrwbbgugwgwgrwrwwgbubgrurubg
bbwubbwgbwggbuwuwrgbbwububrggwgwwgwgrgbubbwuguugwgguubg
uugbubbwbbbwrbgrwwrgbbwwwgrrgubggbuurbbgwgrggurbu
uwgrwggbrbwgrwbuugbuugbbwubbgwrbbububuwrbgbrgrrwr
bburgbuwbuuuwuurgggbbgrruruwgbwuwubgubuguggguuwwbbg
rgugrbrrbrrwgwguguwgurugbubuwbrurwrwuggrruuggbrgugrwgrwubg
wggrbgwburwrbbubwuwwrgurrugrruggwuubbubwgbwburwrug
rugwbuurwuubwwrrgubuwbrrbrbugggurrrggburgbwuwgbbgruugguugb
wgrbbubugwgwbbbrguuuwwbwgrbbrwbburrbbuuugguggr
wwwgurwwuruuuuwgbwwgwbwruwbgrugbgwrgwuggurrugbgubbrwugubr
uugwbwrubrbggrguwwbbrrgrbbgbrwwwgrrgbgwgggrrggbbggg
ruugbuwurwgurrbwrggubwwburwgubuggwrgbgbrbrbbuurwwwrrbgu
bburgbggrgurugbwuururbwubggrrggubbwwrbgugrb
rburrgubuuuubburguubggwuwgbbwrwrubwrubrurgwr
brrrwguwgbuggwwggbrrwbbwguwbrgwuwbwgwugwbwwbbbrubg
bwwbwwuruubwrgbwwgrrguburrbugwbwubwbwbuuguuwggw
bbbgguwruwrurwuwwgrwgwgbubrgbwbrwguuuuuguuwrrggwbbwbub
rrrurrwrgwrwwbbrbrbgbbuwbgbrwgbwbbbbrrwrbuwrgbuwwrguguwubg
wgbgrrrbrrwburuubgrrugbbrwwguwgwgbgruurgwbuugugggbrrubg
wwurgwwrugbwbgrbwbwgwuwrugubgwwrugwugwbbrwrwbbubuubbwurwgw
bugbgrrbrrrwrbwrrurwrrrwgrbuurwuggrggubgrw
ggwgrgruububwbwuuruwwwbuuguuuwrgrugwwbbgrrrrwgrwb
gbrrwwrrgwubrrgurgbwgrrrrgwbgrggbrburbrubg
wwwurrgwrrwgugrruwwgbbbuuwuuugrwgwugrwwwugbrwuu
wrwbbwubbrbugrrurgbrbgbuwgrguububbugbrrwuguggubggrubugu
ubwggwrubwuwgrrgubwugwwbuwrbbwrwgwwuuubguubuwrwgugwwubg
rbgwrrgwwrubwrgbwwuwgubwbbgburwwgwubrwrubg
brwwbgwuwrrwwwuggwbuggbuwuuwbwwrwuwrrbbuwugwgrrgr
bwbgugwwubuuwbrrwrurrbuwgrwbbbwbuurguurubrwgruwgggrbrgb
bguwrgubgwwrwwbbgbguruubbubruwgggrwrgwuguwbwgwuurugrrww
wbggubrwwgwuwururgrwrguurgrubggbgbgbugbwurgggguwwwr
wurgbrrrrwubrgwgbbgwrwrwurrbgugwgwuwubrwbwbwrurgb
bugbgrbuguuuurrgbburugurggrbrrrugrbggwrrgrggggubburubg
ugrwggubgbubburwubgbubwwuubgbbbwggrbuugguuubg
gggbbubbrrgwuburgbwgurbrrwgbrwgrwubrbbgrgubg
bgwrrwgbwwgrbbrgbuwwwbwbubbrguwugubwuwrwggb
gbbburbwruruubwubwrbggbggwgbwubrwrrrbrbrwbur
bbwwbrubwgurgrwbgrrrbubgrbrbbwrrwrrgbuuwgwubbuwgwbbugggru
rbguggwburbgrwubuwggruwbuwuwrggrgbbrwwgwgurgubg
urwgrggbbbbbubbwbbbbwggggbugubbggrrbwguuubugguuruuubbwuruw
wwwrwgwwwuugwggugwwwubwwwbuwgrgwbrgrubwrrubg
wrwbwrubgruwugrgrwwrgbubbururwuwrugbwuwwruuuwbr
brwrgbguurbwbwbrbrggbwurwwrugwbrrwbbgrburwbwrwrrgggubg
rubuuwuugrbrwrwgrubrrwbwuwbgwuubwugbrwwguubg
rrbgurburwgrgrgubrrgwwuugwubgrrgrbgwwwubwgubg
gwgbuwruuggbrbugggwwuugwburrwbgwugubgbubgwgwwr
wbbwrrbwgbruwbbgrrgrubrgugwgwurbgruuwubwubg
buuuwgbgruuugbggwwuwgguwbwwbgwurgbbbgggrwrbug
ugbwuubggrwwrbgbburwuwgrrgrggurwwwwgubrubwubg
ugrugbruggrbrbbggbbgurbbwbbubbrggwurbwubrw
bbbrggruuwrwgwbwuwugggbwwbgubrbbububrgbgwbgrbwgurbwrrgb
rwgrbwrbugubrrwwgrruggurrwbggrbuwrrwgrgbwuu
gugwrbbggbbuuggwrrwbrbrrbrrggbugrbugbrbuug
rwwrrrbruwwbwubwbugbbwuwgugwurgrrbwwurgugwwbrggrubw
rrugwuggwgwubuwubgruugrguwwbgrrrbgbubbgbbrwuuwwwgruburuuubg
ggubugugwbwrwgwrbbbwugugubbugrrwurwrbrrwwrgwgrr
wuwrubwrwrurgwbrbrurwbwbwwrwugrgrwwgwwbwbbgggbw
brbguwbwbgubbruubrwgwbbrbgwwrgggwrwrrbbrrrggrwgbugbgrrb
ggbwguwgrgbwrbubrbbuuguguwuuugwbwgrwbugwggrw
wggwwurrbggwwurggurgwgbwurrgbrwbbgugbwgrbwrbbbrgrru
ubbwbbrbrwgwrrrgbwrrwgrbggubbbuuuuuugubwgubbrgbrbbggggrguu
bwuurgrugbgguggbrbgbgwbbgubwugguwgrggrwgwwgrgbugubg
uruburwurrrbuubgbrwbwrrbgurrggrbgrrwgwwwgurubg
rubbubrgrbuguwguwrgbbrwrbrbbwggggbwruwugwugwrrgubgrubg
wuwwuugrurrwwuuwwbbuubgwbugwrgwrgwurugruuwgrgbrubg
uwrgguwbwgbgbgguurwguubwrburrwrwgbbbugrrubrwburbrguu
rwwbrgrgrbwgburubwbburbbbbuwuuuwgwwuwrwwrrugwrwbbrguwgubg
ubrgguuugbwuwrwwbbwgbbgbrrurwuwwgrwrbwwbbuuggggurgubg
wugbwugrbrbuguuwuwruwurugubwurbwugrwwbuwugrrrgwbbgwgwb
gruuuwrbgrrbrrrwurwgwwgrugubugwubwrurwgrugbrwrburu
rbbrwrwgubrrwbwrgbggwuwgrbbbgugrbuwbbwwrwbwgwbuwgwbrw
wwwwrwbrgrwggrbbuurwgrbrrwrwrwrbgbgwwrubg
ugrbugubwugwbgburgurrggbgbbwgggrwuuwguuggwuuubg
bbgwuugbwrubbrrwbrggbbbuwbwbuubrgrbrbbuwgrwwgbrrubrwr
rrgubuwuwbgwgwuwubrwwwwgwuggrrgggbwbbruubg
bwbgbuggrwgbwrgwuwuwbgbbrrbbuugwugrrgwbwwgubrbrgbugubg
ggruruwruwwuwgrbwgbwurbbrbrgugwbuubwgbuwrwurgbr
wuwwrwuugguwwwbwgggbwbwwbuugbuggbrbubrgubgugrubwgrbgbur
gwwbbbggrwrrubwrbugbrrwrbguwwwugbgrggrbubwwubg
ubwbrrwrugwuggrgrwwwugbugwrguuwubbugrbwubggbg
ggrwrgbwbugbrgwuburbwwgrbwwrrbgwuwuggrgwbu
uwwuggwubgurgrugbbbbugrrwgwwuurwrwbwguurrugrwwggrb
uwuubguugwrgwrugrururgubrburuggugbuwubrrwrwrguubg
rgggbubbubrgrugbuwgubbuwuggbrgurwbuuurwwrgbrgugwubg
guguwwrrubbuuurbuuwwwwrwubbguwwubgrrrugggrb
gbggruugbwwruubrbguuwrwrgwwwuuwbguwrrrrwbgrgbbwgwuu
rwggbgwrrgbwrrugbgurwwrrbburwrbgguguwuguwubuugwruuwur
wuurguurwbbrgwbrbbugwuuuuggwgrbbugruwbwbuggwbrbwbbbgggr
bgwbwwbwgwburruwbuwwbuguggugwwrugrbgbbbgubg
wrwgurwuuurwwruwwbguwrrbwrgrugbrgugwrbrubggrrgwwburgruurr
ubgrbwwugruuwbbwuubgbbbwrurbbrruwbubrgbggrbbwuuwubg
wbrrgubgwrbguwbgrwruugugggwgwugbggubwbrggrwwgbgrwu
gbwurgurbburububgrruwbrrubbgubwwbbbbgbguubg
gwbrugurgwwrruubgbbguwgwgrbwrgwgbrrbrbwwwurrwgubg
brwbuurggwuuwbbrrurwugbbrrbrburgubrwgwbgrbwubbuubwurbwubg
gwbbwbwuuwuruwgrwrwgwrgggbuwbrrbwwbrwgwggbggrbwggwubwb
bwgrbgrwgwwwuurrrggruuwubgbbwrgrrubuwurrbugrbwubgugggwr
ububuggwwurruggrgbubuwgruwgwgwwrbgrwbubwugwuwgbwurrrgwbur
wwuruwrwgwrbgguubrwuwuwburgwgwwbrwwburwuurr
ugbgbugrbuggrrrwgugbgrwgrbbwguuruugrgwrgburwbgbugbg
urgrwrrgrruggugbbrwwwrgwggwurgbwbrgrwwrbuwuuwbggwgw
bubugbrrruggwgubugbuugwwubugwwgbbruwbrgrbbubwbruwgwubg
brwrrrwwubgwwbuwbrrrbubrwrgurwurrrgrbbwubgw
rurwgrbguugugwrwwwuuuwwbrwuggrbugggubwuurwbu
ruubwwwguguwugbbrgwwubrrrburwrggggrguwubrugbw
rgggurrwubbuwwrrrrwggbwrrgrguuurrwbbrbrbubgugwruubg
wbrubgwrwbgugwwbggwrbbruuwgbwbrurrwrwrwgbrb
rwrubruwbgbbuugguwgburwwgwruuuuuwwwwugugwbggguwbubrwruubg
ubwurugwrruwrbrbrwwgubggrwbrguuwruuubuubggruwugrrwbbgbr
urgrubbubbgurbwuurgrgrggbwubwbgubwuwgugwgrgwrbr
rrbbwwuugrubrrrbwwgwgubbwbruwuwwwbwruurbubwuwwrurrrwrbbru
ggbwbbubrwurwwwwbrgbruwwgrrgggrbggbrgurwwrbwggrur
gbbgbruwuwgrrgrrgwwwbwgwwgburgrbwbbrururguurgwuuwrbwbrbg
bwbrwrurrbbwrggwrrrwrugrbruwbrbuwrrguwwwgrbugb
urruubrwbbgbbgbgbugwburggrrwuuwbuuwugubwuwrrgwbrbbgwbrbuwu
burbruurgrgrugburrbbwwbwrgwbbwrggbwbwuuuugrgbuurbbrggubg
wbgbbuwrbrgwbgbbggbrubwrbbbwgubugbubrgruwwrggrwubg
guwurwurgggbbbwrurbgwbubrrwubbbgrubrwwgbuwuububbguuruu
wububrwuwrgwruwugrwgguugruuuuurrwbrwubbgubg
gbwbugbwgbbrubrbguggrwuuuubrbugbwgubrubrwgw
uuwburgrbwrbwurwrwgbrrgrguggbbwbgbbuuuwbrwbgb
bbrgurbggrbugrrruwuuuwuuubbubugububrwurwrbrguwwug
rrbruwuruugrrrrgwgrwbbguwgwguuwugugwgbgrrgbruubbwrwg
wrgrgubbwwrbbuuuwrggrwguwgubgbwbubggrwrubgrgwwubg
rbwwrbrrurgugbggbwrgrggwbubgbrgrruwrbguguuuurrbwggrwgwgubg
wuuurbbguwuubbgwbrrwrgubugwrwwggbbbgbbrrggb
wwwubrwgbgurgubuggwwbuwwrubguurgwguubbwuwgbbggbgurrrubg
rwuubugrrwwrugwgubwwugguburggugwgbbuwuwgugwgbugbggrwubg
ugugrgruubuuwwubwwrbgrbruwrwugwrwuwggruwggwbwrrrr
rbbguurwuubwrwbburruubuugbgugubgwggbggbgrrruwrurbwuggguubg
rwgwbuuuwrrugwbbbwgwrgbwuwwwbbbrwuubrgrurwwgw
bugbuurrugbrbburrggrbwbwuwwgbwwuuwgbwwgrgr
guwuuwrgugbrgwrbwwrrububruubuwwbwugwgugwrrbuwuwugrwubgubg
wgwbrubgrwugwguguuuruwbrubbuubbuugrbrbuugubwrrur
wbrbbwbuwubuwbubgurbbwwwgurwwguwgurrurbgwbrrrrgubg
ugguubrgrwwbugrbrwrbbwubwgbbbruubwbgbbwuwruubg
ugggbwbwrugwbwrubgwggugwruurwugrgugwgurrbbwbrww
rwurwgbrrbubwrbuurguuuruugbuwgwwrrwbgguubbbgbr
uwrrggwbwbwwbbwrurrgwbwwwwwubrwbgugrrgwgwbbbu
wurgwbbbgwrrubbbrrurubrbrugbwbwbwrwrrbrrrggwwwbrrbrbubu
grubruwugbggggwbgwwrwbugrgbwbwwwguurrubuwgubg
rrwbggruugbugubwbwrrrwrbguuuugwbgrgwbwrwgrbrrbwrruugwuwwr
ubrubwrburwwwgwbrrbuwwbbubggwuruubwbgwuwurubburbgrwugwr
wuwbbrgrgubrrbwurwuguurwrwuwgbuggwrgwwrrwrrwuuruubggwgbrw
gurbgbggwrgbguuruuugggwrgrrwrwgwgurgugugwwggwr
rrbwgwbbbrgguggurwubbggwwwuuwbbrbwrrggrrurbrbwubrurbwwuguu
uuwggwwrbrrbwrgurgrrgggrbgwuwburwwbggubggubg
rggurwuurwugwubbggwggbrbrubrrwgrwrrbrrrugrwrgrw
rbguwgwwbbgbwbuugrwbbuwwguwuwwwugruggwrbwguurw
burgwuuwuugugurgwurwuggbgwgrwwuggrubbrrgwbbbwbruugugrrggr
wgwgrugrgwruggrbwwggbuggbbrrbbggggrrrbbbuubbrgbgrbbwwurbu
gubgwwwwbbbuwguwruuubgwgwbgwuwguguburbrgwwwguubg
ubrwgrwrbwrrggguwgwurbwbbwugbrwrbbwgbbuuuwgbwuggbugbgrrgubg
grggggwgbubwbbbgggrwuubuwwbuguwrbrwgubbggrbww
urgurubbbbrbbgguubbuuwrgruggrbbwwwrrrwwrbrbbwrrbggg
ububuwruwburrwgwrbrrguwwbgbgbwggurrgrburubbgurgu
bbgwgubbuurrwgrgwurbugwburbwrrgurbwbrugwrgubg
buwwrbuggwrgurgubugggbwggrwggggrwuwbgwggrrrurw
uwbrwruwwwwgbbubbgwbuuwuwbbugguwwwwrguuwgbbgguwgubrrburubb
uwrrbwwwrwuugrwrbwbwgurwwwguwuuuuwggugbrbubbrbwwuuwwgrubg
rgwwwguugbuwbgwgrrbwbruugrbbbgurggwrrgruuggbgbuwubwrbgww
bwuwrwwrbbrbgwrbbbuuggwbbbwgbwwgwrrgbubbgwrrwbrubwwgbwrb
rrbgubgwguwurggbwurruwbwrrwrgwwgugrwgwwrbwggrruguubwbguggu
ggrbwgbbbbguugbbbubrrbwwrruwuwrgwbrggwwrrbwrwubg
wguuugbgrrrubwwrbggubbgbrugbrwuuwbgwbbbgbuugwwburw
wgbwbwwgubrrwwrwuwgbrurwwbwrgrgwbguuuwuurbgrbwburrgurwgwr
gwggwwbwggurrrbuuwbgugwgbrbugwrbugugggggwbwrubwuuuwrgbw
gwbwuwguwgrgrugbgruuugubgbgrwbwgwrwgwubgubg
buuggwwbrbugrrurbburgbbubrugwburwurwugruurbgwbuwgrbwrgubg
gbwuurbggwugubggurgbgrguuggbgurggbuugubgwgrwwurrgwrgubg
wubwrrgbgbbwburbuuwuugbwggwrwrgwwbguurgugwuwrrubg
ugruuruuwrburwwuuuwwbwrbwrwwgburrwwbguwgwrgrgrbgurburg
wwwwwgrggwgrugbgrubbwgurgwuubuuguwwbbggwbrwbwubg
gwrugrgbugbbgugrrrbrwuwguurbuuguubwwggubg
uwbubrwwbrggrrrubwwwrwbrwrgwguugrrbbbrurubg
bbgbuuwwggrbugbuburwwubrbubuuuuwburwwbuurbgwgwgugbrwgubbgw
rwrgrruggrgubrwuwwwwburwwrguugwgruwuugggrgwur
gggwbugwrwurrgwgwugwbugrbrrrrrwruwgbruruwwgububurbwrgbwggu
rwwggbgubbbrrurwuwrrbwruwruubruurwrgbubuggbuwrrrgguugrbuwr
bgguguwgrugubwgwbbbrwbuwrbrbwwrbwwrrubwrur
brrwwgbbbrbuurwrugbubuwgubbrwwwguurrgrbugrrr
ugugurwuugrwrbgwrgbwgrruuwugrrwwuggrbggbgrburbgwbwrwuwwug
wrgbbrugbrbwbgwrwgrrurwrurbugwrwgbgbggwuwggbbggu
wbwgrgwgbburgguwugwwguggrbgbwrbrubuguubg
wwrbububruggbwrgrwguwggugrrbbbrgubwrgruggubg
urgrgbggugubrggrbrrubbgbbrbrrwwrwggbwgrgbbgruruwbg
guwbgrwbbwrgubwugbbbugbrurugbbbguugbbbrgrrbubwwwuwbrww
bubgbwbrgubbrgubwrbwrgwwbruwrbwgruwbwwgbbbrwuggbbubugbr
wugrwugrrubbbgwubuububuubbrrurgwrrbuuwugbbrrg
gwrbbruwgbuuurrrbgbubbgbgbrrgurwbggbwrgbruwbgrwb
wwbgrwbuwwgbgbgbwugwuwgbrrubbrgurrbwbwrgwgbggbwubuwubg
gbrrgwgrrurrbuurgggugwbguuuguurggwwuwwrwuwbgwuwuuwuwgubg
wrwugwwrgwrbrrrbguwgbgwbrubwugrwguuwgugrwwwwrgugwrgwbw
rwrwgubrrrgrgurbggurgbrgwuubbubwgbgurgwbuurrggbwb
uwggwbggbgrwubrrrgwrwgbubwwbgbgbguuugguguuwwwrgrgwwrwwgubg
rrrwbuwrwwgwbwbgbrgrgwwruwguwggwrgrbbrubg
wgwuruurubuuwrugrurwwbwuggwrgwuwbuggwrruwugwwbwwgurwbrbbw
rrgburuuuwrgruwurrbwbwrwgrgrrwggwggwrguwgwbgubuwgu
wwwubgrrbwbrwwuwuggbrubrwugrrwggrurbuuugwurgguwwur
wuruuuwrrrbubbrruwwburguuugwgubbbrwwrwrgbgrgrrgugwrwbbb
gbwbbrguwuuugwwwwrubuuubgrwbbuwrgbuuguuwwwwbwwbuwgwrubg
bgwgwggurbrrbgrrbwubgguwrwbgruwugurbbwwurugrwrbwrubwbwwbr
wbwrwrbuubgururbwwwgrrwgwwbrugbubwwurgwrrg
bgbbwruugbbgrwggwbrrwwbbuburrrwgburubbrbguub
buwuuuuguwwrwgrgubuwgruwugrwgggurgbuguwwggggrurggw
grwuggrrrrbbwbwurgrwrwuwwgbgrbgbburggbuuuwwurubuugwurwuubu
rurburuwwrbbbwbbgugwbwrurwgwbbrgwrwguuwbbgrbruwurr
uburbbwwrbbrguwugugbrbrgbrwbubuuubwuubrwggrugugwrgwgwrbw
ggwubggruuwrburrwrwrrwuurgbwuguggrwrwuggugg
bwbgrburrubrbbuwbbrrrggrgrwrrgwuuwuwggubg
gwgbggurrruguugggggbguwrbguubbbuwurggrbrwrbgggbr
bwwwbrwgggruuruuwwrrugururrwrbrwbgbbbubbwwr
bbrbwwgwgugwuwwbrbbrbruwgwgbbbwubgbwguubuubuwrruwgg
ubbrubgwwwugbrugbggwrgwugwwgbbggbggburugubg
guguwrrwwggbwgwuugrgwgbwbrurgggwguggrubg
uuugubrgwrugwgbbwwrubwgwggbgbwuurrugggrbubbuw
rgbwwwugbrbwgwwuwwrbubggrggrwuurrbwgbggrruubw
ugrurgururugrggbbgbggugwwbgwbgguwbwwbbgbrguwbrwbbbguwrbb
gurbbbubgwwgbburwuubwbggbububguuwugwgbgrwbbuuubbruug
uwurrwubbbrwguwuwgugwurwbuggrgrurguubrgugbgurgbbwgwgwg
bbrbgrrggbuguugwbwggbgbuwuurggrubugggbuuururruuwbgwubg
gggwrugrrrwrwbwbbgbwuggubwbgbbggubuubgbbwbrg
uubrwrrugbrgrwgwgrbwgrugggubbubguwgbwrubgbggwgbgwrwgg
wrwrwgrrbrwbrugubggwwwwurwrbubwgwggbrbwwubg
gwbrgwrbggrguruubgwugubruggurrbwwuwwbgbrrwrguuwbbbguwr
ggrbwrrrurwrwbugrguuuwurgrbwrbugwrwbbgugbwbrbgbbwuruubg
rbrgbubgbubrggbwuwgruuwwbbwwrburuuuuugwwwbwurwburwrwwgbb
ubwurbruurgwubrbgbwrrburguugbbwugurgrbgruu
buubgggubbggrrrwrggbbugwbburwbwrgrgbbwbrbrbwbwgubg
rruwwuubbgwgbubbbubugggburrwgrwugwuguuwrrwubg
wurrwurbwbubbgrrggwbwrwrwubwrugruruwwgbgwgwgrw
rgwgrrurbbguuubgwuugubwrwwbrwbgugbrwrbgrbrrbb
uuwgwuuuurbguwgggrbwruuuwgrrbgrugrrrugwgugubbrww
bbbrrgugwurggwurwbgubggbgrurgwuwuwbrrugrggrgwrbrbrbgbr
grrrbbburbgbrrwrwugrgwgbrrrguwbwbrruwrubwg
wburgbwgrburguruurgggwgrwggbwuwwbwbwgwrgwwuwuguurbugur
wwbrrgwuuwuguugbrwbwrrbuwwrugbgbrgwuruwbrwubr
rbbggubrguwwwugbbubwurbwuwgbrbwugbrgubggguwbwbuwubg
uurruwruwbgbrrbrgbgrgbugbrubwrwurgwggrwugbruwrwubwggb
urugwbbwwrugwuwugrgrwrrgbburwgwwrbrbgwbrbrbgggrbrgruubg
buruuruggggbbgruugrrugggwuugruuwbrwwwuugwrubgrwwguwr
ugwguuwrwgwwbbrwbggwbwubgrgrwgubbguwbrbrggbbggrwgwbwgr
gbruuuurugbgwurbrgubbbuggwuruuubrwwgwrwugggrug
ggbubbbwugrwbwgbrrrbwgwbwwrwrurwwgruruwwbrbbr
uwgggurrrrgbwguwwbrrurrrbguguwugrgbrbuwugwubwgburb
bgrrwuuubgbwbbuubwrwwubbwwuwbwrbbrurgguggrwuggubg
buwugwurwbrruuurbbbbrguburrwgbbwgruugwuugwgbbugwggwur
wrrgggwuwgugurrgbbbuwrruuuubgrbrbuugrwbuuggrgwbrwurubg
rgggrwbrgwguwburbgbugrgubbguwbwrbwubwwwrgbbuwrwbrrburg
guwbbrgwbwuugruuruggubrguuugwgwrgwuggwbgubwbbuwrrubg
gbrwurwubgbrrbuwwrgburbwggbuwbbwwubuwggwbbrbrguurgubg
ugburwgwguuuguugrggrgwuwrgugwgwuruwugrrgurbugguww
wbbubbubgbbbbwbrbwgrbrrubwgrrbuwuggwgbbwguu
uubruuwgbgguwbrggrgbrgbruruwgwbwrrbuurruwrurbwbbrr
uuggrbuggubwwurbbburgwubrwwugubbrggbwbugrbrggbwugubrgwubg
urwbbbwggrwgwggbgwbwwggrwgggubuubggbbwrgwbgrwgwbwbg
wugrwrbuwwrbwbgbubugwbgwbwbrrrwbwbgrwubrgbwbwrguwbwburguubg
rwggrruuruuuwuubgwugubbwbwugwwugggbrrbrwwggbruugbg
brrwwwubgwbubugrrguuwbrwwgururburbbbbrbgwrbbrubbbug
gbbrwuwgwbgbubwrwburrbbwbbuwwgrrbuuwurubg
uubrggurugugurbuurgbbbururubbgguubgrruwgurburggbgww
gbwwgbgruwuuuwwgggwgrbuurrgubuuwguwuuuuwwggwuwurbwubu
gurrbgwuuugrgguubbwgbgurbwwruwwggrrgwwgwggggrg
wbubwgwurrrgrguwrrubrwwgbugbburbgwburwgwwrrbrguuurubg
gggrgugwugrrgrgrguwwuuuuubrbbbguwbggguburwgwbubgwwgu
rwbbgrurwuggbwgwgugbuuugrwgbwwrwubrwgrurgrwbbggrbrrwbwgwrubg
rbruubuwggrgwubwguuwwubguwurbguuwbuuwgbrwrbwrrbrg
gurgubwrwrbrgruwgruwbrbgwgbggggubwrguurruwruuugw
brbbggrrrbwrgwwwwrggbuwurbbwbggurbrbbbwuuuggubg
uwbgbgrwbbwbguguggbbbgbuwwgwuwwbgurubbgugrrggwbrrwrrurgw
wururrbbbbrgggrrgbbgwrgwgrwwwrbbwgubgbuwuuurubg
rgubrrbbwwwggrrwgwrwbbwrwwwgbgbbrruggugrbuwgugwbrr
wuwguggubbwwwbuuguuugurgwgrrwruuggugbgrwguwuub
rbwwurubbwuwubbggurgbwwbubrrubbwuurwwrwgrbubw
bgruubugbrgbgbuuwubrbgbuwrguuuurbuwguubgguubwbrwb
rugugurrwbuwbgwbwurwwgwrrurbbuwbgruugrguwrwuwbwwgbwwrwrgw
rwurggrbrwbuggguuuggggrgbbgwwrrbwggugwuurrr
ruwubbggwrgwrggwbrbgggubwugbuggwubbubwbruuwwrw
wruwbbrubwrrbrbgurguuwbuggrrwbrrgrwrwwrubwwwwubbww
urrrwuubwbubwrguubgbwuubgrgwrbugubrwwbgrrrwb
wgwuwrwgggurrwwguwrurwbwuwubububgbgbwwbwggwbwbgwrgbrwbwu
rwgugwubburuubwwguwwruwbrbgguwrbgwbbwrgugrbgw
rrbwgurbgwuwwwbgggubwguwgbgwwbuggrrubuwbggbguggwggb
ruggwgbgrburwrgrrbrrurwwgwwrbrggrrgrbrgbgwrubg
rwguwrugggrugbwwrggrrbbwgbugggrgwrwrgwgwuwbuuggbwww
ugbwggguwwggwugbuuwbrrgurubrrgwgburrbguwuwuwwuurbwu
ugurgwbgbuwwrrwgggbrwwruburrrgbrrbburggruruwgrrurbb
wrgbwwrgguubrgrwrwrbwuwuuruwuburgwbgurwgurbubbguwrguggwrr
rugugrgubbgwbrwgggurbruggbburbugwbbwwguurgrbrgubbrub
gbwguuuwrrwwwwuwbrwbbwwruuwruwwbggbbwrgubrrrggbrbgrurgwubg
rburugwrggrbwuwggwburgurrwrgruubbbwgubgubg
ugubbgrgbwggururuuwbruuuugrgbubrugwbgrrgwubg
rguurrrubbrwburggburrbbgguggrwggrgrguwbrurrrgu
rbwrrbggwwuubbbwgrrwbrrwrwggugrgbubburwgbrgbbuuuggbrg
rbbubgurwwgwwggbbwwwbgwbbubruguwwwrrgbwggg
grbwgruugguwgwbrrgbrwwggurbwgugwgbrrurbwrwbgurgg
wuruwwubuubwugwrgwwgguugbgbrgguuubgbrrbwbuwbwwuwub
brbwbgwwrrwwwrruwbgrbwbrrbrrwbwuubbbbrrbwbuubbrurbruwrbr
ugwbrgrgugbgubrwwrgbrgbugbgwrrbgwruwuburruwwbgrrrggubggu
wgbwbwgwbwuuwbugguuwgwrruggruuuggwuwrgwwuuubbrrgggbbu
uwbrguwgururruuubuwgbwbuuwgbwgbugwrwrwbwuwrwwgwur
bgrgbrwgwrwbgbgwwrwbwugurbgbgguwbuuwubwugrubg
bgbwbrgguruwubrrwrwbbwuuugbuguguubgggbwwrugwwb
rwbggbggrrrgggwugwuwwrwrwugbrbwrbgbgbgwggrgrwbwrgw
buwrruuwubrrgrwbgbrwgbbrbgrbbgrrbrgbgrgrwbrwwrruuwbugwgr
rgrbuuwrrwwubbgwggbubwrrbwuguubbugwrbuugruwrgrgwrwg
bgwgbwgrbbgrgbrbuguuubwugrwgbrubrwbbbwggubwbur
rwbbgbwwrrbruwuwbrrbbubwwbwwwbgbgwurwubg
wubrgwugwbwugwrbbruurwuwgugwburuwbbrgubbrwwwbbwrw
ugubwwrwwbrrbbrwrwuuuwburrrrwwbuuubwguwguuwbwurug
brgrugbugbuurgugbwgwubbrubwbugguuuurbbbwrb
grgruggugbbbwurgrwrwrwuuubugbbgggrrwwbwbggb
bwrwuuuuuwbwruwwwrubrburrrrburgubbgubgbbrgguwwwbgw
guburrbubwrrwwwubgrguruwwuugwubwrgwubrrubugwgbgurubg
rwrrwbwwgwgubggurrbbuwrbubwrwwwurwugrgubrrbbw
rbuwbwbbwrgruuugwrwgwruubuubbguggrguuubuubburrgguurrgbrw
wubbwrgbbbruwrgbbrbgugbgbuggbgbrbrbguwwgwgrwgbwrgb
wurrbbrburgbgbrbgbwruwuwwrgrrggbgbrguwbggwgbgwrrubg
grrwwgwwbgwbugurrgbbwrgwbugwrrwugggwururubrurwguuur
ururwugguggbuurgbwrwbuwrwburruwwrurrrgubbrbrbwrruurbwgrug
grrbruuuuwuwwwrrwubwgguwgbruuuwgrrbwwurgwubg
wwbguwrbwubggrurrgruurwgwbruwrubrbwubrwrwwgguwgwwbwgwbuubg
guwgbrrbggbgwuubwguurubrgwgubrrgbrwbwubburuubrwbgubrrbwgubg
rwgrwuubuwbbguruburuggugrurgbuwwrurbrggwwwrwwwuwrbbbbgubg
ggrgwwwuwbwgburggurwwugwrwburuubwbwguuubg
wbbgbgwgwgrrgwbruuurwurwbgwbubwuruwuurbbgrubuuwbgubg
bgguggrrrgbwuubgrwburubggrgwuuuggwwubbrgrrgbrwbwbguwbw
grbubwguruugrgwrurubwwggbbwbbrurbgggbwguwubuwwwwwrwwwr
brgbuuugwbbuburrrbrbgrwugwbgwgrbgbrugwbrrwgrubg
rggbbbuburrruwuuwbrbbwrurburrurwgwbrrrbbrwrwgrbuwrgruubg
uwrwgwgwurwuwgrrrwrbuurgguruwwurwuurggugguwgbbrwbrbuu
wwruwrwrwwwwuruuuuuuuuuurgwbwwuuurrwrwgbgrgubg
rrbgbrwwwwbgurubrgwgbwwgggrurwugrwrwruguubrwurgurbgbgwubgb
gurrwrrrbwgwguwbrbgrrbrbbbuugrrrubggbuwrwuug
gbrbwrgubwrggguwbrugbbwgwuwrwwggbuwwuugrwbgbugbuggrrbgrubg
gugruuuuuwbgbgrrbbggrrwwuuwwgrwrrwbuggurrgwrbg
bwrbgruggbrbbbguurgurrwggbrubrbrubwwbrgguw
rrwrgrwbggurbwrruwbbugruuwgbrgrwbwruuguwbrbrugrruwwwgggww
rbguubuwgrugurugrbbubwubwwwubwgwwrggurbwurwuw
rrgguwwrwrgrbrwbwgugurgbrrrwuwrggbwrbbbrbgbbb
rrrwbuuubuwgrubrurbbwburrrwrgwbgbwgbbugrrrrrw
rwugbrgbbwbwugrbrrubwbrubbwwwuwbrgrwgbuggbrwwwrwurrubg
grrwwuwbrbggrwwwwrbbbbbburbubrubgwwbwgwrubwuubgrwrbwruurwubg
rugubrbwwrurgrrbgrubrwbbugubrrbbbuuwbwburgbuwbbggwrrrbrubg
rgwgwuubbwuwrbbwrbwubwgbrgggbgurbrurrguuubg
uwuuuubrgbbrgbwgrbbwrrbwbgurbwuwrbuwgwrwwru
uwubuggbuurwruuwrugbbwgggwgbgbuurgrgrrbbwbguwbuugbgwwwuw
ubburgbwubgubguurugwruwrurbwbgurwuuwbbbbgggwg
ubwwrbuwgbwwwrwguubbbrbggwururuugbbuggrrugwrrwgwwbbggruubg
"@
    }

    $patterns = [System.Collections.ArrayList]($patterns -split ', ' | where-object { -not [string]::IsNullOrEmpty($_) } | sort-object { $_.Length })
    $needles = [System.Collections.ArrayList]($needles -split "`r?`n" | where-object { -not [string]::IsNullOrEmpty($_) })
    $min = $patterns | Select-Object -first 1 | ForEach-Object { $_.Length }
    $max = $patterns | Select-Object -last 1 | ForEach-Object { $_.Length }
    $layers = $min..$max | foreach-object {
        $length = $_
        $layer = [pscustomobject]@{
            ItemLength   = $length
            Items        = $patterns | Where-Object { $_.Length -eq $length } | Sort-Object
        }
        $layer | Write-Output
    }
}

