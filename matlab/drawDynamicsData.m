h=figure;
%each dim q qd qdd
disp('draw fig for q qd qdd-----------');
for i = 1 : 12
    plot(model.q{1,i}, 'r');
    hold on
    plot(model.qd{1,i}, 'g');
    plot(model.qdd{1,i}, 'b');
    name = strcat(num2str(i), model.jtype{i});
    name = strcat(name,'.fig');
    saveas(h, name,'fig');
    clf
end
disp('end draw fig for q qd qdd----------');