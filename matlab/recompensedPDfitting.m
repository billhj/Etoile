%torque regression
h = figure;
disp('torque regression-----------');
mdl_recompensePD = cell(12,1);
for i = 1 : 3
    q1 = model.q(i, :);
    q2 = model.qd(i, :);
    x = [q1', q2'];
    tau1 = model.tau(i, :);
    y = tau1';
    mdl_recompensePD{i} = LinearModel.fit(x,y);
    mdl_recompensePD{i};
    plot(tau1, 'red');
    hold on
    plot(mdl_recompensePD{i}.Fitted, 'blue');
    name = strcat('recompensedPDfitting_', num2str(i));
    name = strcat(name, model.jtype{i});
    name = strcat(name,'.fig');
    saveas(h, name,'fig');
    clf
end

for i = 4 : 12
    q1 = model.q(i, :);
    q2 = model.qd(i, :);
    tau0 = model.tau(i - 3, :);
    x = [q1', q2', tau0'];
    tau1 = model.tau(i, :);
    y = tau1';
    mdl_recompensePD{i} = LinearModel.fit(x,y);
    mdl_recompensePD{i};
    plot(tau1, 'red');
    hold on
    plot(mdl_recompensePD{i}.Fitted, 'blue');
    name = strcat('recompensedPDfitting_', num2str(i));
    name = strcat(name, model.jtype{i});
    name = strcat(name,'.fig');
    saveas(h, name,'fig');
    clf
end

disp('end torque regression-----------');


% for i = 12 : -1: 10
%     q1 = model.q{1,i};
%     q2 = model.qd{1,i};
%     x = [q1', q2'];
%     tau1 = model.tau{1,i};
%     y = tau1';
%     mdl_recompensePD{i} = LinearModel.fit(x,y);
%     mdl_recompensePD{i}
%     plot(tau1, 'red');
%     hold on
%     plot(mdl_recompensePD{i}.Fitted, 'blue');
%     name = strcat('recompensedPDfitting_', num2str(i));
%     name = strcat(name, model.jtype{i});
%     name = strcat(name,'.fig');
%     saveas(h, name,'fig');
%     clf
% end
% 
% for i = 9 : -1: 1
%     q1 = model.q{1,i};
%     q2 = model.qd{1,i};
%     tau0 = model.tau{1, i + 3};
%     x = [q1', q2', tau0'];
%     tau1 = model.tau{1,i};
%     y = tau1';
%     mdl_recompensePD{i} = LinearModel.fit(x,y);
%     mdl_recompensePD{i}
%     plot(tau1, 'red');
%     hold on
%     plot(mdl_recompensePD{i}.Fitted, 'blue');
%     name = strcat('recompensedPDfitting_', num2str(i));
%     name = strcat(name, model.jtype{i});
%     name = strcat(name,'.fig');
%     saveas(h, name,'fig');
%     clf
% end