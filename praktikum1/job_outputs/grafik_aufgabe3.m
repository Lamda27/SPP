%% Grafische Darstellungen für Aufgabe 3
laufzeiten = [0.0121569330; % sequentiell
    0.0120733980; % parallel (1 Thread)
    0.0086913200; % parallel (2 Threads)
    0.0052090220; % parallel (4 Threads)
    0.0050150550; % parallel (8 Threads)
    0.0051945020]; % parallel (16 Threads)

threads = [0,1,2,4,8,16];

% bar plot
figure(1);
bar(threads,laufzeiten*1000);
xticklabels({'Seq.','1','2','4','8','16'});
xlabel('Anzahl paralleler Threads');
ylabel('Laufzeit (ms)');

% logarithmisch
figure(2);
loglog(threads(2:end),laufzeiten(2:end)*1000,'-x');
xlabel('Anzahl paralleler Threads');
ylabel('Laufzeit (ms)');