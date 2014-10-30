%torque regression
disp('torque regression-----------');
mdl_originalPD = cell(12,1);
for i = 1 : 12
    q1 = model.q{1,i};
    q2 = model.qd{1,i};
    x = [q1', q2'];
    tau1 = model.tau{1,i};
    y = tau1';
    mdl_originalPD{i} = LinearModel.fit(x,y);
    mdl_originalPD{i};
    plot(tau1, 'red');
    hold on
    plot(mdl_originalPD{i}.Fitted, 'blue');
    name = strcat('originalPDfitting_', num2str(i));
    name = strcat(name, model.jtype{i});
    name = strcat(name,'.fig');
    saveas(h, name,'fig');
    clf
end

disp('end torque regression-----------');