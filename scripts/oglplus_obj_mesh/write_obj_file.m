% Export vectors and faces to obj file
% 
% Parameters:
%  - vectors is a mx3 array to save m vectors to the file
function write_obj_file(filename, vectors, faces)

    fid = fopen(filename,'w');
    
    fprintf(fid, '# Stefan Urban <stefan.urban@live.de>, 2015\n');
    fprintf(fid, 'o Plane Example\n');
    
    % Print 3 coordinates for every point
    for i = 1:size(vectors, 1)
        fprintf(fid, 'v %.6f %.6f %.6f\n', vectors(i,1), vectors(i,2), vectors(i,3));
    end

    % Faces can have different dimensions to support multiple geometries
    % (triangle, quad, ...)
    for i = 1:size(faces,1)
        
        % TODO: support for anything which is not a quadrad
        fprintf(fid, 'f %d %d %d %d\n', faces(i,1), faces(i,2), faces(i,3), faces(i,4));
        
    end
end