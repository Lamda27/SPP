%% Grafische Darstellung für Aufgabe 5
laufzeiten = [12.6293;
    6.7073;
    3.6273;
    2.1598;
    1.8332;
    7.8544];
threads = [1,2,4,8,16,32];

% bar plot
figure(1);
bar(threads,laufzeiten);
xticklabels({'1','2','4','8','16','32 (16)'});
xlabel('Anzahl paralleler Threads');
ylabel('Laufzeit (s)');

% logarithmisch
figure(2);
loglog(threads,laufzeiten,'-x');
xlabel('Anzahl paralleler Threads');
ylabel('Laufzeit (s)');