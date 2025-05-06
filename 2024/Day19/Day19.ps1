Process {
    
    # Process all the Input
    $Result = $needles | ForEach-Object -begin { 
        $percent_current = 0
        $percent_total = $needles.Count
        $progress = @{
            Status = 'Testing inputs'
            Activity = 'AdventOfCode 2024-19.1' 
        } 
    } -end { 
        Write-Progress @progress -Completed 
    } -process {
        Write-Progress @progress -CurrentOperation $_ -PercentComplete ($percent_current / $percent_total * 100)
        $needle = $_
        [pscustomobject]@{
            N = $percent_current++
            Needle = $needle 
            Status = (aoc2024_19_1 -needle $needle -layers $layers)
        } | Write-Output
    }
    
    # Display the figures:
    $Result | Group-Object Status

}
Begin {
    function aoc2024_19_1 {
        param(
            $needle,
            $layers,
            $level = $null
        )
        # compute some valuable values 
        $max = ($layers.ItemLength | Measure-Object -Maximum | Select-Object -ExpandProperty Maximum )
        $min = ($layers.ItemLength | Measure-Object -Minimum | Select-Object -ExpandProperty Minimum )
        if ($null -eq $level) {
            $level = $max   # if not set, set l to the max allowed value
        }

        # out of bounds => fail
        if ($level -lt ($min)) { 
            return $false
        }
        # not enough chars, reduce $level
        if ($needle.Length -lt ($level)) {  
            return (aoc2024_19_1 -needle $needle -layers $layers -level $needle.Length)
        }

        # Find substring in available patterns
        $ss_end = $needle.Substring($needle.Length - $level)
        $ss_match = $layers | Where-Object { $_.ItemLength -eq $level } | ForEach-Object {
            $_.items | Where-Object {
                $_ -eq $ss_end
            }
        }
        # match found
        if ($ss_match) {    
            if ($ss_match.Length -eq $needle.Length) {  # the whole input was processeed, done for good
                return $true
            }
            # there is more to process, move next
            if (aoc2024_19_1 -needle $needle.Substring(0, $needle.Length - $level) -layers $layers) {
                return $true    # Analysis of the remaining string = success, done for good
            }
            # analysis of remaining string failed
            # let it try something else...
        }
        # no match, or analysis of remaining failed, try something else
        return (aoc2024_19_1 -needle $needle -layers $layers -level ($level - 1))
    }
    <#
    $patterns = 'r, wr, b, g, bwu, rb, gb, br'
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
#>

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
    $patterns = [System.Collections.ArrayList]($patterns -split ', ' | where-object { -not [string]::IsNullOrEmpty($_) } | sort-object { $_.Length })
    $needles = [System.Collections.ArrayList]($needles -split "`r?`n" | where-object { -not [string]::IsNullOrEmpty($_) })
    $min = $patterns | Select-Object -first 1 | ForEach-Object { $_.Length }
    $max = $patterns | Select-Object -last 1 | ForEach-Object { $_.Length }
    $layers = $min..$max | foreach-object {
        $length = $_
        $layer = [pscustomobject]@{
            ItemLength   = $length
            Items        = $patterns | Where-Object { $_.Length -eq $length } | Sort-Object
            Permutations = @()
        }
        <# $layer.Permutations = for($i = 0; $i -lt $layer.Items.Length; $i ++) {
            $current_permutation = [pscustomobject]@{
                Item = $layer.Items[$i]
                Permutations = @()
                Perfect = $false
            }
            for($j = 0; $j -lt $layer.Items.Count; $j ++) {
                $current_permutation.Permutations += , "$($layer.Items[$i])$($layer.Items[$j])"
                if($j -ne $i) {
                    $current_permutation.Permutations += , "$($layer.Items[$j])$($layer.Items[$i])"
                }
            }
            $current_permutation | Write-Output
        }
        #>
        $layer | Write-Output
    }
}

