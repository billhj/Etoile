h=figure;
disp('draw fig for q qd qdd-----------');
%each dim q qd qdd
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

%torque regression
disp('torque regression-----------');
for i = 1 : 12
    q1 = model.q{1,i};
    q2 = model.qd{1,i};
    x = [q1', q2'];
    tau1 = model.tau{1,1};
    y = tau1';
    mdl = LinearModel.fit(x,y);
    mdl
end

disp('end torque regression-----------');