%torque regression
disp('torque regression-----------');
mdl_accurecompensePD = cell(12,1);
newFitted = cell(12,1);
for i = 1 : 3
    q1 = model.q{1,i};
    q2 = model.qd{1,i};
    x = [q1', q2'];
    tau1 = model.tau{1,i};
    y = tau1';
    mdl_accurecompensePD{i} = LinearModel.fit(x,y);
    mdl_accurecompensePD{i};
    newFitted{i} = tau1;
    plot(tau1, 'red');
    hold on
    plot(newFitted{i}, 'blue');
    name = strcat('accumrecompensedPDfitting_', num2str(i));
    name = strcat(name, model.jtype{i});
    name = strcat(name,'.fig');
    saveas(h, name,'fig');
    clf
end

for i = 4 : 12
    q1 = model.q{1,i};
    q2 = model.qd{1,i};
    tau0 = newFitted{i - 3};
    x = [q1', q2', tau0'];
    tau1 = model.tau{1,i};
    y = tau1';
    mdl_accurecompensePD{i} = LinearModel.fit(x,y);
    mdl_accurecompensePD{i};
    newFitted{i} = mdl_accurecompensePD{i}.Fitted';
    plot(tau1, 'red');
    hold on
    plot(mdl_accurecompensePD{i}.Fitted, 'blue');
    name = strcat('accumrecompensedPDfitting_', num2str(i));
    name = strcat(name, model.jtype{i});
    name = strcat(name,'.fig');
    saveas(h, name,'fig');
    clf
end

disp('end torque regression-----------');